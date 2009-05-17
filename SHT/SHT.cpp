/*
  SHT.cpp - Library for SHT Temperature Sensors
  Author: Dan Willemsen, dan@danw.org
  Copyright (c) 2009 Dan Willemsen.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "WProgram.h"
#include "SHT.h"

#define CMD_READ_TEMP  B00000011
#define CMD_READ_HUMID B00000101
#define CMD_READ_STAT  B00000111
#define CMD_WRITE_STAT B00000110

#define nop() __asm__("nop\n\t");

SHT::SHT(int clockPin, int dataPin)
{
    pinMode(clockPin, OUTPUT);
    _clockPin = clockPin;
    _dataPin = dataPin;
    last_temp = 0;
}

void SHT::startTransaction()
{
    pinMode(_dataPin, OUTPUT);
    digitalWrite(_dataPin, HIGH);
    digitalWrite(_clockPin, HIGH);
    
    digitalWrite(_dataPin, LOW);
    digitalWrite(_clockPin, LOW);
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_dataPin, HIGH);
    digitalWrite(_clockPin, LOW);
}

void SHT::sendByte(byte out)
{
    byte mask;
    
    pinMode(_dataPin, OUTPUT);
    
    mask = 0b10000000;
    while(mask)
    {
        digitalWrite(_clockPin, LOW);
        digitalWrite(_dataPin, ((out & mask) != 0) ? HIGH : LOW);
        digitalWrite(_clockPin, HIGH);
        mask = mask >> 1;
    }
    
    digitalWrite(_clockPin, LOW);
    pinMode(_dataPin, INPUT);
    while(digitalRead(_dataPin) == HIGH);
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_clockPin, LOW);
    
    while(digitalRead(_dataPin) == LOW);
    
    mask = 0;
    while(digitalRead(_dataPin) == HIGH)
    {
        mask++;
        if(mask == 255) break;
        
        delay(10);
    }
}

uint16_t SHT::readWord()
{
    byte i;
    uint16_t ret = 0;
    
    digitalWrite(_clockPin, LOW);
    
    pinMode(_dataPin, INPUT);
    
    for(i = 0; i < 17; i++)
    {
        if(i != 8)
        {
            nop();nop();nop();nop();
            digitalWrite(_clockPin, 1);
            ret = (ret << 1) | digitalRead(_dataPin);
            digitalWrite(_clockPin, 0);
        }
        else
        {
            pinMode(_dataPin, OUTPUT);
            digitalWrite(_dataPin, LOW);
            nop();
            digitalWrite(_clockPin, HIGH);
            nop();nop();nop();
            digitalWrite(_clockPin, LOW);
            pinMode(_dataPin, INPUT);
        }
    }
    
    return ret;
}

int16_t SHT::readTemperature()
{
    startTransaction();
    
    sendByte(CMD_READ_TEMP);
    
    int32_t temperature = readWord();
    int32_t celcius;
    
    // This assumes the 14-bit setting
    
    // 3.3V
    celcius = -396 + temperature / 10;
    temperature = -394 + (18 * temperature) / 100;
    // 5V
    // celcius = -401 + temperature / 10;
    // temperature = -402 + (18 * temperature) / 100
    
    last_temp = celcius;
    
    return temperature;
}

uint16_t SHT::readHumidity()
{
    startTransaction();
    
    sendByte(CMD_READ_HUMID);
    
    uint32_t humidity = readWord();
    
    // Compute Relative Humidity
    // Do this all with integer math, we only really care to a certain precision anyways
    uint32_t humidsq = (humidity * humidity) / 1000;

    humidity = -20468 + 367 * humidity - 16 * humidsq;
    
    // Adjust to ambient temperature - extremely simplified (TODO - Better?)
    if(last_temp != 0)
    {
        humidity = (last_temp - 250) * (12) + humidity;
    }
    
    humidity = humidity / 1000;
    
    return (uint16_t)humidity;
}

uint8_t SHT::readStatus()
{
    startTransaction();
    
    sendByte(CMD_READ_STAT);
    
    uint16_t status = readWord();
    
    return (uint8_t)(status >> 8);
}