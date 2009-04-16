/*
  Expander.h -  PCF8574 expander library
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
#include "Expander.h"
#include "Wire.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/
Expander::Expander(uint8_t address)
{
  this->address = address;
  
  this->write(0x00);
}

/******************************************************************************
 * User API
 ******************************************************************************/
    
void Expander::write(uint8_t value)
{
  Wire.beginTransmission(this->address);
  Wire.send(value);
  Wire.endTransmission();
}

uint8_t Expander::read()
{
  uint8_t value = 0x00;
  Wire.requestFrom(this->address, (uint8_t)1);
  if(Wire.available()) {
    value = Wire.receive();
  }
  return value;
}
