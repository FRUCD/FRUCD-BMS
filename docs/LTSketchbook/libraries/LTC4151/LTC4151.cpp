/*!
LTC4151: High Voltage I2C Current and Voltage Monitor

@verbatim

The LTC4151 is a high side power monitor that operates over a wide voltage range 
of 7V to 80V. In default operation mode, the onboard 12-bit ADC continuously 
measures high side current, input voltage and an external voltage. Data is 
reported through the I2C interface when polled by a host. The LTC4151 can also 
perform on-demand measurement in a snapshot mode. The LTC4151 features a 
dedicated shutdown pin to reduce power consumption. The LTC4151-1/LTC4151-2 
feature split I2C data pins to drive opto-isolators. The data out on the 
LTC4151-1 is inverted while that on the LTC4151-2 is not. 

@endverbatim

http://www.linear.com/product/LTC4151

http://www.linear.com/product/LTC4151#demoboards

REVISION HISTORY
$Revision: 2040 $
$Date: 2013-10-15 16:18:50 -0700 (Tue, 15 Oct 2013) $

Copyright (c) 2013, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However, the Linduino is only possible because of the Arduino team's commitment
to the open-source community.  Please, visit http://www.arduino.cc and
http://store.arduino.cc , and consider a purchase that will help fund their
ongoing work.
*/

//! @defgroup LTC4151 LTC4151: High Voltage I2C Current and Voltage Monitor

/*! @file
    @ingroup LTC4151
    Library for LTC4151: High Voltage I2C Current and Voltage Monitor
*/

#include <Arduino.h>
#include "Linduino.h"
#include "LT_I2C.h"
#include "LTC4151.h"
#include <Wire.h>

// Write one byte to an LTC4151 register.
int8_t LTC4151_write(uint8_t i2c_address, uint8_t adc_command, uint8_t code)
{
  int32_t ack;
  
  ack = i2c_write_byte_data(i2c_address, adc_command, code);
  
  return(ack);
}

// Reads a 12-bit value from LTC4151
int8_t LTC4151_read_12_bits(uint8_t i2c_address, uint8_t adc_command, uint16_t *adc_code)
{
  int32_t ack = 0;
  
  ack = i2c_read_word_data(i2c_address, adc_command, adc_code);
  
  *adc_code >>= 4;
  
  return(ack);
}

// Note that the following functions are somewhat contrived, and the units are arbitrary.  LTC4151_ADIN_voltage could be given an LSB
// weight (LTC4151_adin_lsb) in volts instead of mV, and the returned voltage will be in volts instead of mV.
// Similarly, the LTC4151_sense_current function could be written to accept an LSB weight in amps or milliamps.
// That would eliminate the extra floating point divide operation based on the sense resistor value.

// Calculates the LTC4151 sense current in Amps given "resistor" value in ohms and "LTC4151_sense_lsb" LSB weight in volts.
float LTC4151_code_to_sense_current(uint16_t adc_code, float resistor, float LTC4151_sense_lsb)
{
  float voltage, current;
  voltage = (float)adc_code * LTC4151_sense_lsb;    //! 1) Convert code to voltage from sense lsb
  current = voltage / resistor;                     //! 2) Calculate Current, I = V/R
  return (current);
}

// Calculates the LTC4151 V_IN voltage given "LTC_vin_lsb" LSB weight in volts
float LTC4151_code_to_vin_voltage(uint16_t adc_code, float LTC4151_vin_lsb)
{
  float voltage;
  voltage = (float)adc_code * LTC4151_vin_lsb;  //! 1) Convert code to voltage from VIN lsb
  return (voltage);
}

// Calculates the LTC4151 ADIN voltage in mV given "LTC4151_adin_lsb" LSB weight in mV
float LTC4151_code_to_ADIN_voltage(uint16_t adc_code, float LTC4151_adin_lsb)
{
  float voltage;
  voltage = (float)adc_code * LTC4151_adin_lsb; //! 1) Convert Code to voltage from ADIN lsb
  return (voltage);
}
