/*
  RTC.h - External DS1307 RTC library
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
#include "Wire.h"
#include "RTC.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(uint8_t val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
uint8_t bcdToDec(uint8_t val)
{
  return ( (val/16*10) + (val%16) );
}

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/


RTCTime RTCClass::time()
{
  RTCTime tm;
  
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.send(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  tm.second     = bcdToDec(Wire.receive() & 0x7f);
  tm.minute     = bcdToDec(Wire.receive());
  tm.hour       = bcdToDec(Wire.receive() & 0x3f);  // Need to change this if 12 hour am/pm
  tm.dayOfWeek  = bcdToDec(Wire.receive());
  tm.day = bcdToDec(Wire.receive());
  tm.month      = bcdToDec(Wire.receive());
  tm.year       = bcdToDec(Wire.receive());

  return tm;
}

void RTCClass::set(RTCTime tm)
{
  RTC.set(tm.year, tm.month, tm.day, tm.dayOfWeek, tm.hour, tm.minute, tm.second);
}

void RTCClass::set(uint8_t year, uint8_t month, uint8_t day, uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t second)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.send(0);
  Wire.send(decToBcd(second));    // 0 to bit 7 starts the clock
  Wire.send(decToBcd(minute));
  Wire.send(decToBcd(hour));      // If you want 12 hour am/pm you need to set
                                  // bit 6 (also need to change readDateDs1307)
  Wire.send(decToBcd(dayOfWeek));
  Wire.send(decToBcd(day));
  Wire.send(decToBcd(month));
  Wire.send(decToBcd(year));
  Wire.endTransmission();
}

RTCClass RTC;
