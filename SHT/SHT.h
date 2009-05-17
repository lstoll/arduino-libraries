/*
  SHT.h - Library for SHT Temperature Sensors
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

#ifndef SHT_H
#define SHT_H

#include <inttypes.h>

class SHT
{
public:
    // Args: clock pin, data pin
    SHT(int, int);
    
    // Read the Temperature
    int16_t readTemperature();
    
    // Read the Real Relative Humidity from the SHT
    // This value is an integer, but shifted by ten, so instead of XX.Y, it's XXY
    // This always compensates for the non-linearity of the SHT
    // If a temperature reading has been completed, it will compensate for the temperature as well
    uint16_t readHumidity();
    
    // Read the Status register
    uint8_t readStatus();

private:
    // Pin assignments for clock and data pins
    int _clockPin, _dataPin;
    
    // The last temperature sensor reading, used for
    // Temperature compensation of Humidity Signal
    // (in Celcius)
    int16_t last_temp;
    
    // Initialize a Transaction
    void startTransaction();
    
    // Send a byte to the SHT
    void sendByte(uint8_t);
    
    // Read a word back from the SHT
    uint16_t readWord();
};

#endif