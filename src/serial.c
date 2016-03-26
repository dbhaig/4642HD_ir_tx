/* serial.c
  Copyright 2015-2016 Don Haig (time4tux at gmail dot com)
  MIT Licence (See LICENSE.txt for details) 
*/

// Serial interface routines

#include "serial.h"
#include "commands.h"

String inputString = "";         // incoming data
boolean stringComplete = false;  


void SetupSerialInterface()
{
  Serial.begin(baudRate);
  Serial.println(idnString);
  inputString.reserve(serialBufferSize);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if ((inChar == '\n') || (inChar == '\r')) {
       stringComplete = true;
       break;
    } else {
       inputString += inChar;
    }
    if (inputString.length() >= maxCommandLength) inputString ="";
  }
}

boolean SerialInputReady()
{
  serialEvent();
  return stringComplete;
}
 
String ReadFromSerialInterface() 
{
  String tempString ="";

  if (stringComplete) {
    tempString = inputString;
    inputString = "";
    stringComplete = false;

  return tempString;
  } 
}

void SendToSerialInterface(String message) 
{
  Serial.println(message);
}
