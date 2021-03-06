/*!
LTC6903: 1kHz to 68MHz Serial Port Programmable Oscillator

@verbatim

The LTC6903/LTC6904 are low power self contained digital
frequency sources providing a precision frequency from 
1kHz to 68MHz, set through a serial port. The LTC6903/
LTC6904 require no external components other than a 
power supply bypass capacitor, and they operate over a 
single wide supply range of 2.7V to 5.5V.

@endverbatim

http://www.linear.com/product/LTC6903

http://www.linear.com/product/LTC6903#demoboards

REVISION HISTORY
$Revision: 3018 $
$Date: 2014-12-01 15:53:20 -0800 (Mon, 01 Dec 2014) $

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

/*! @file
    @ingroup LTC6903
    Library Header File for LTC6903: 1kHz to 68MHz Serial Port Programmable Oscillator
*/

#ifndef LTC6903_H
#define LTC6903_H

#include <SPI.h>

//! Define the SPI CS pin
#ifndef LTC6903_CS
#define LTC6903_CS QUIKEVAL_CS
#endif

/*!
@{ 
|   OCT Options     |   f>=  (MHz)  |   f< (MHz)    |
|   :-----------    |   :--------:  |   :-------:   |
|   LTC6903_OCT_15  |   34.05       |   68.03       |
|   LTC6903_OCT_14  |   17.02       |   34.01       |
|   LTC6903_OCT_13  |   8.5111      |   17.01       |
|   LTC6903_OCT_12  |   4.256       |   8.503       |
|   LTC6903_OCT_11  |   2.128       |   4.252       |
|   LTC6903_OCT_10  |   1.064       |   2.126       |
|   LTC6903_OCT_9   |   0.532       |   1.063       |
|   LTC6903_OCT_8   |   0.266       |   0.5314      |
|   LTC6903_OCT_7   |   0.133       |   0.2657      |
|   LTC6903_OCT_6   |   0.0665      |   0.1329      |
|   LTC6903_OCT_5   |   0.03325     |   0.06643     |
|   LTC6903_OCT_4   |   0.01662     |   0.03322     |
|   LTC6903_OCT_3   |   0.008312    |   0.01661     |
|   LTC6903_OCT_2   |   0.004156    |   0.008304    |
|   LTC6903_OCT_1   |   0.002078    |   0.004152    |
|   LTC6903_OCT_0   |   0.001039    |   0.002076    |
*/
// OCT Options
                                // f>=  (MHz)       | f< (MHz)
#define LTC6903_OCT_15  0x0F    // 34.05            | 68.03
#define LTC6903_OCT_14  0x0E    // 17.02            | 34.01
#define LTC6903_OCT_13  0x0D    // 8.5111           | 17.01
#define LTC6903_OCT_12  0x0C    // 4.256            | 8.503
#define LTC6903_OCT_11  0x0B    // 2.128            | 4.252
#define LTC6903_OCT_10  0x0A    // 1.064            | 2.126
#define LTC6903_OCT_9   0x09    // 0.532            | 1.063
#define LTC6903_OCT_8   0x08    // 0.266            | 0.5314
#define LTC6903_OCT_7   0x07    // 0.133            | 0.2657
#define LTC6903_OCT_6   0x06    // 0.0665           | 0.1329
#define LTC6903_OCT_5   0x05    // 0.03325          | 0.06643
#define LTC6903_OCT_4   0x04    // 0.01662          | 0.03322
#define LTC6903_OCT_3   0x03    // 0.008312         | 0.01661
#define LTC6903_OCT_2   0x02    // 0.004156         | 0.008304
#define LTC6903_OCT_1   0x01    // 0.002078         | 0.004152
#define LTC6903_OCT_0   0x00    // 0.001039         | 0.002076
//! @}

// Output Configuration
#define LTC6903_CLK_ON_CLK_INV_ON   0x00    //!< Clock on, inverted clock on
#define LTC6903_CLK_OFF_CLK_INV_ON  0x01    //!< Clock off, inverted clock on
#define LTC6903_CLK_ON_CLK_INV_OFF  0x02    //!< Clock on, inverted clock off
#define LTC6903_POWER_DOWN          0x03    //!< Powers down clocks

//! Writes 2 bytes
//! @return 1 if successful, 0 if failure
void LTC6903_write(uint8_t cs,      //!< Chip Select Pin
                   uint16_t code    //!< Code to be written
                  );

//! Calculates the code necessary to create the clock frequency
//! @return the code for the LTC6903 
uint16_t LTC6903_frequency_to_code(float frequency, //!< Clock frequency in MHz
                                  uint8_t clk       //!< Output configuration
                                 );

#endif

