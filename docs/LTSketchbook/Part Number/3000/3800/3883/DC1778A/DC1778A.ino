/*
Linear Technology DC1778A Demonstration Board
LTC3883: Single Phase Step-Down DC/DC Controller with Digital Power System Management

@verbatim

NOTES
  Setup:
   Set the terminal baud rate to 115200 and select the newline terminator.

@endverbatim
 
http://www.linear.com/product/LTC3883
http://www.linear.com/demo/DC177A

REVISION HISTORY
$Revision: 3220 $
$Date: 2015-03-03 14:04:24 -0800 (Tue, 03 Mar 2015) $

Copyright (c) 2014, Linear Technology Corp.(LTC)
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

#include <Arduino.h>
#include <stdint.h>
#include "Linduino.h"
#include "UserInterface.h"
#include "LT_I2CBus.h"
#include "LT_SMBusNoPec.h"
#include "LT_SMBusPec.h"
#include "LT_PMBus.h"

#define LTC3883_I2C_ADDRESS 0x50

// Global variables
static uint8_t ltc3883_i2c_address;
static LT_SMBus *smbus = new LT_SMBusPec();
static LT_PMBus *pmbus = new LT_PMBus(smbus);

//! Initialize Linduino
void setup()
{
  Serial.begin(115200);         //! Initialize the serial port to the PC
  print_title();
  ltc3883_i2c_address = LTC3883_I2C_ADDRESS;
  print_prompt();
}

//! Repeats Linduino loop
void loop()
{
  uint8_t user_command;
  uint8_t res;
  uint8_t model[7];
  uint8_t *addresses = NULL;
  
  if (Serial.available())                          //! Checks for user input
  {
    user_command = read_int();                     //! Reads the user command
    if (user_command != 'm')
  	Serial.println(user_command);
  
    switch (user_command)                          //! Prints the appropriate submenu
    {
  	case 1:
  	  menu_1_basic_commands();                 // Print single-ended voltage menu
  	  break;
  	case 2:
  	  pmbus->readModel(ltc3883_i2c_address, model);
  	  Serial.print(F("MODEL "));
  	  Serial.print((char*)model);
  	  Serial.println();
  	  break;
  	case 3:
  	  res = pmbus->readPmbusRevision(ltc3883_i2c_address);
  	  Serial.print(F("REVISION "));
  	  Serial.println(res, HEX);    
  	  break;
  	case 4:
  	  pmbus->enablePec(ltc3883_i2c_address); 
          delete smbus;
          delete pmbus;
  	  smbus = new LT_SMBusPec();
  	  pmbus = new LT_PMBus(smbus);
  	  break;
  	case 5:
  	  pmbus->disablePec(ltc3883_i2c_address);    
          delete smbus;
          delete pmbus;
  	  smbus = new LT_SMBusNoPec();
  	  pmbus = new LT_PMBus(smbus);
  	  break;
  	case 6:
  	  addresses = smbus->probe(0);
  	  while(*addresses != 0)
  	  {
  		Serial.print(F("ADDR 0x"));
  		Serial.println(*addresses++, HEX);
  	  }
  	  break;
  	case 7 :
  	  pmbus->resetGlobal();
  	  break;
  	default:
  	  Serial.println(F("Incorrect Option"));
  	  break;
    }
    print_prompt();
  }

}

// Function Definitions

//! Prints the title block when program first starts.
void print_title()
{
  Serial.print(F("\n*****************************************************************\n"));
  Serial.print(F("* DC1778A Demonstration Program                                 *\n"));
  Serial.print(F("*                                                               *\n"));
  Serial.print(F("* This program demonstrates how to send and receive data from   *\n"));
  Serial.print(F("* the DC1778A demo board.                                       *\n"));
  Serial.print(F("*                                                               *\n"));
  Serial.print(F("* Set the baud rate to 115200 and select the newline terminator.*\n"));
  Serial.print(F("*                                                               *\n"));
  Serial.print(F("*****************************************************************\n"));
}

//! Prints main menu.
void print_prompt()
{
  Serial.print(F("\n  1-Basic Commands\n"));
  Serial.print(F("  2-Model Number\n"));
  Serial.print(F("  3-Revision Number\n"));
  Serial.print(F("  4-PEC On\n"));
  Serial.print(F("  5-PEC Off\n"));
  Serial.print(F("  6-Bus Probe\n"));
  Serial.print(F("  7-Reset\n"));
  Serial.print(F("\nEnter a command:"));
}

//! Prints a warning if the demo board is not detected.
void print_warning_prompt()
{
  Serial.println(F("\nWarning: Demo board not detected. Linduino will attempt to proceed."));
}

void print_all_voltages()
{
  float   voltage;
  
  voltage = pmbus->readVout(ltc3883_i2c_address, false);
  Serial.print(F("LTC3883 VOUT "));
  Serial.println(voltage, DEC);
}

void print_all_currents()
{
  float   current;
  
  current = pmbus->readIout(ltc3883_i2c_address, false);
  Serial.print(F("LTC3883 IOUT "));
  Serial.println(current, DEC);
}

void print_all_status()
{
  uint8_t b;
  uint16_t w;
  
  b = pmbus->readStatusByte(ltc3883_i2c_address);
  Serial.print(F("LTC3883 STATUS BYTE 0x"));
  Serial.println(b, HEX);
  w = pmbus->readStatusWord(ltc3883_i2c_address);
  Serial.print(F("LTC3883 STATUS WORD 0x"));
  Serial.println(w, HEX);  
}

void sequence_off_on()
{
  pmbus->sequenceOffGlobal();
  delay (2000);
  pmbus->sequenceOnGlobal();
}

void margin_high()
{
  pmbus->marginHighGlobal();
}

void margin_low()
{
  pmbus->marginLowGlobal();
}

void margin_off()
{
  pmbus->sequenceOnGlobal();
}

void menu_1_basic_commands()
{
  uint8_t user_command;
  
  do
  {
    //! Displays the Read/Write menu
    Serial.print(F("\nRead/Write\n\n"));
    Serial.print(F("  1-Read All Voltages\n"));
    Serial.print(F("  2-Read All Currents\n"));
    Serial.print(F("  3-Read All Status\n"));
    Serial.print(F("  4-Sequence Off/On\n"));
    Serial.print(F("  5-Margin High\n"));
    Serial.print(F("  6-Margin Low\n"));
    Serial.print(F("  7-Margin Off\n"));
    Serial.print(F("  m-Main Menu\n"));
    Serial.print(F("\nEnter a command: "));

    user_command = read_int();                              //! Reads the user command
    if (user_command == 'm')                                // Print m if it is entered
    {
      Serial.print(F("m\n"));
    }
    else
      Serial.println(user_command);                         // Print user command

    switch (user_command)
    {
      case 1:
        print_all_voltages();
        break;
      case 2:
        print_all_currents();
        break;
      case 3:
        print_all_status();
        break;
      case 4:
        sequence_off_on();
        break;
      case 5:
        margin_high();
        break;
      case 6:
        margin_low();
        break;
      case 7:
        margin_off();
        break;
      default:
        if (user_command != 'm')
          Serial.println(F("Invalid Selection"));
        break;
    }
  }
  while (user_command != 'm');
}


