/* serial.h
  Copyright 2015-2016 Don Haig (time 4 tux at gmail dot com)
  MIT Licence (See LICENSE.txt for details) 
*/

#ifndef serial_H
#define serial_H

#include <Arduino.h>

#define serialBufferSize 25 
#define baudRate 115200 

#define maxCommandLength 15 

#ifdef __cplusplus
extern "C"{
#endif

  void SetupSerialInterface();
  boolean SerialInputReady();
  String ReadFromSerialInterface(); 
  void SendToSerialInterface(String message);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
