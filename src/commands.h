/* commands.h
  Copyright 2015-2016 Don Haig (time4tux at gmail dot com)
  MIT Licence (See LICENSE.txt for details) 
*/

#ifndef commands_H
#define commands_H

#include <Arduino.h>

const char idnString [] = "*IDN Arduino - Cisco 4642HD Remote Control";

#ifdef __cplusplus
extern "C"{
#endif

    void ProcessCommand(String commandString);
    void EchoHelpInfo();
    void EchoIdnString();
    void TogglePower();
    void SetChannel(String commandString);
    void EchoLastChannelSet(String commandString);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
