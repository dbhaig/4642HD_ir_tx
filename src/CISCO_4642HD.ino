/* CISCO_4642HD.ino
  Copyright 2015-2016 Don Haig (time 4 tux at gmail dot com)
  MIT Licence (See LICENSE.txt for details) 
*/

#include "serial.h"
#include "commands.h"
#include "irtx.h"

// ISR declaration required
ISR(TIMER0_COMPA_vect)
{
}

// ISR declaration required
ISR(TIMER1_COMPA_vect)
{
   IrTxSender(); 
}

void setup() {
    SetupSerialInterface();
    SetupTimer0();  // Modulation 
    SetupTimer1();  // Bit Length
} 

String serialInput;

void loop()
{
    if (SerialInputReady()) {
       ProcessCommand(ReadFromSerialInterface());
    }
}
