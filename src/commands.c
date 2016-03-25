/* commands.c
  Copyright 2015-2016 Don Haig (time 4 tux at gmail dot com)
  MIT Licence (See LICENSE.txt for details) 
*/

// Command parsing routines

#include "cisco.h"
#include "commands.h"
#include "irtx.h"
#include "serial.h"

const int numberOfCommands = 5;
const int maxChannelDigits = 3;

String lastChannelSet = "000";

const String command [] = {"?",
                            "*IDN?", 
                            "POWER",
                            "CHANNEL",
                            "CHANNEL?"
                            };


const String description [] = {" - Outputs this help menu.",
                                       " - Returns identification string", 
                                       " - Sends IR transmission to toggle 4642HD power",
                                       " [0-9][0-9][0-9] - Sends IR transmission to set 4642HD to specified channel",
                                       " - Returns channel specified in last IR transmission"
                                      };

void ProcessCommand(String commandString)
{
  commandString.toUpperCase();

  if ( commandString == command[0] ) { 
     EchoHelpInfo();
     return;
  }

  if ( commandString == command[1] ) { 
     SendToSerialInterface(idnString);
     return;
  }

  if ( commandString == command[2] ) { 
     TogglePower();
     return;
  }

  if ( commandString != command[4] && commandString.startsWith(command[3]) && commandString.length() > command[3].length() ) { 
     SetChannel(commandString);
     return;
  }

  if ( commandString == command[4] ) { 
     EchoLastChannelSet(commandString);
     return;
  }

}

void EchoHelpInfo()
{
  int i;

  for (i = 0; i< numberOfCommands; i++) {
      SendToSerialInterface("#   " + command[i] + description[i]);
  }
}

void TogglePower()
{
  String code = onOff;
  IrTx(code);
  SendToSerialInterface(command[2]);

}

String getChannelParameter(String commandString) {

  commandString.trim();

  String channelString = commandString.substring(command[3].length());

  channelString.trim();

  return channelString;
}

boolean ValidChannel (String channelString) {

    if (channelString.length() > maxChannelDigits) return false;

    for(char i = 0; i < channelString.length(); i++) {
       if ( !isDigit(channelString.charAt(i)) ) return false;
    }

    return true;
}

void SetChannel(String commandString)
{
  String channelString = getChannelParameter(commandString);

  if (ValidChannel(channelString)) {
    for (char i = 0; i < channelString.length(); i++) {
      String code = digits[channelString.charAt(i) - '0']; //convert char to int
      IrTx(code);      
    }
    lastChannelSet = channelString;
    SendToSerialInterface(command[3] + " " + lastChannelSet);
  } 
}

void EchoLastChannelSet(String commandString)
{
  SendToSerialInterface( commandString + " " + lastChannelSet );

}

