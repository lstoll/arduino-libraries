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

#ifndef LM35DZ_h
#define LM35DZ_h

#include <inttypes.h>

class LM35DZ
{
  private:
    uint8_t pin;
    
  public:
    int min;
    int max;

  public:
    LM35DZ(uint8_t pin);
  
    int read();
    int sample(uint8_t, uint8_t);
};

#endif

