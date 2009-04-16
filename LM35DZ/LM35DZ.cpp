/*
  LM35DZ.h - LM35DZ temperature sensor
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
#include "LM35DZ.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/
LM35DZ::LM35DZ(uint8_t pin)
{
  this->pin = pin;
  this->min = 255;
  this->max = -255;
}

/******************************************************************************
 * User API
 ******************************************************************************/
int LM35DZ::read()
{
  int temp = (500.0 * analogRead(this->pin)) / 1024.0;
  
  if (temp > this->max)
    this->max = temp;
  if (temp < this->min)
    this->min = temp;
  
  return temp;
}

int LM35DZ::sample(uint8_t samples = 3, uint8_t dl = 500)
{
  int sum = 0;
  
  for (uint8_t i = 0; i < samples; i++)
  {
    sum += this->read();
    delay(dl);
  }
  
  return sum/samples;
}
