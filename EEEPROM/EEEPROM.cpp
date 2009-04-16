/*
  EEEPROM.h - External (I2C) EEPROM libray.
  Copyright (c) 2009 Amadeusz Jasak.  All right reserved.

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

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "WConstants.h"
#include "EEEPROM.h"
#include "Wire.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/
EEEPROM::EEEPROM(int address)
{
  this->address = address;
}

/******************************************************************************
 * User API
 ******************************************************************************/

uint8_t EEEPROM::read(unsigned int addr)
{
  uint8_t rdata = 0x00;
  Wire.beginTransmission(this->address);
  Wire.send((int)(addr >> 8)); // MSB
  Wire.send((int)(addr & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(this->address, 1);
  if (Wire.available()) rdata = Wire.receive();
  return rdata;
}

void EEEPROM::readPage(unsigned int addr, uint8_t *buffer, int length = 16)
{
  Wire.beginTransmission(this->address);
  Wire.send((int)(addr >> 8)); // MSB
  Wire.send((int)(addr & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(this->address,length);
  int c = 0;
  for ( c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.receive();
}

void EEEPROM::write(unsigned int addr, uint8_t value)
{ 
  Wire.beginTransmission(this->address);
  Wire.send((int)(addr >> 8)); // MSB
  Wire.send((int)(addr & 0xFF)); // LSB
  Wire.send((int)value);
  Wire.endTransmission();
  delay(5);
}

void EEEPROM::writePage(unsigned int addr, uint8_t *data, int length = 16)
{
  Wire.beginTransmission(this->address);
  Wire.send((int)(addr >> 8)); // MSB
  Wire.send((int)(addr & 0xFF)); // LSB
  byte c;
  for ( c = 0; c < length; c++)
    Wire.send(data[c]);
  Wire.endTransmission();
}
