/* CISCO.h
  Copyright 2015-2016 Don Haig (time 4 tux at gmail dot com)
  MIT Licence (See LICENSE.txt for details) 
*/

#ifndef CISCO_H
#define CISCO_H

#include <Arduino.h>

#define OCR0Avalue 139    // This will occur every (140*clock speed)  = 8.75 uS

#define OCR1Avalue 12     // This will occur every (13*clock speed/1024)  = 832 uS

#define markLength 836    // usec 
#define codeRepeats 3     // Tx concatenates this many copies of the code for a button transmission  
#define w_count  4        // Number of mark length gaps between codes

// Button Codes (1=ON, 0=OFF for markLength)

const char button_0 [] =  "f088a222a22a8aa22a200000000000"; 
const char button_1 [] =  "f088a22aa2a8a8888a200000000000";
const char button_2 [] =  "f088a222a8aa2a888a200000000000";
const char button_3 [] =  "f088a228a8aa2a288a200000000000";
const char button_4 [] =  "f088a2222a2a8aa88a200000000000";
const char button_5 [] =  "f088a22a28aa2a228a200000000000";
const char button_6 [] =  "f088a2228a2a8aa28a200000000000";
const char button_7 [] =  "f088a2288a2a8a8a8a200000000000";
const char button_8 [] =  "f088a222228aa2aa8a200000000000";
const char button_9 [] =  "f088a22a88aa2a222a200000000000";

const char* const digits [] = { button_0, button_1, button_2,\
                                button_3, button_4, button_5,\
                                button_6, button_7, button_8,\
                                button_9 };

const char     hash [] =  "f088a228a222a2a28aa00000000000";
const char    onOff [] =  "f088a22222aa8aaa22200000000000";
const char     star [] =  "f0a88888888a22aaaa200000000000";

#define binaryBufferSize  sizeof(button_0)*4 

#ifdef __cplusplus
extern "C"{
#endif
#ifdef __cplusplus
} // extern "C"
#endif

#endif
