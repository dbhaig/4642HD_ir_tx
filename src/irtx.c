/* irtx.c
  Copyright 2015-2016 Don Haig (time 4 tux at gmail dot com)
  MIT Licence (See LICENSE.txt for details) 
*/

// Infrared Transmit routines

#include "irtx.h"
#include "cisco.h"
#include "serial.h"
#include <string.h>

const char BinaryString_0 [] = "0000";
const char BinaryString_1 [] = "0001";
const char BinaryString_2 [] = "0010";
const char BinaryString_3 [] = "0011";
const char BinaryString_4 [] = "0100";
const char BinaryString_5 [] = "0101";
const char BinaryString_6 [] = "0110";
const char BinaryString_7 [] = "0111";
const char BinaryString_8 [] = "1000";
const char BinaryString_9 [] = "1001";
const char BinaryString_A [] = "1010";
const char BinaryString_B [] = "1011";
const char BinaryString_C [] = "1100";
const char BinaryString_D [] = "1101";
const char BinaryString_E [] = "1110";
const char BinaryString_F [] = "1111";

const char* const hexToBinaryString [] = { BinaryString_0, BinaryString_1, BinaryString_2,\
                                           BinaryString_3, BinaryString_4, BinaryString_5,\
                                           BinaryString_6, BinaryString_7, BinaryString_8,\
                                           BinaryString_9, BinaryString_A, BinaryString_B,\
                                           BinaryString_C, BinaryString_D, BinaryString_E,\
                                           BinaryString_F };

int freeRam () 
{
   extern int __heap_start, *__brkval; 
   int v; 
   return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

//Modulation Timer                                            
void SetupTimer0()
{
    TIMSK0 &= ~(_BV(OCIE0A));// Disable Output Compare Match A Interrupt

    pinMode (6, OUTPUT);    // Timer 0 "A" output: OC0A (Digital Pin 6)
    digitalWrite(6, LOW);   // Enable Pin 6 and set LOW 

    TCCR0A = _BV(WGM01);     // Wave Generation Mode 2, Clear Timer on Compare (CTC)

    TCCR0B = _BV (CS00);     // No prescaler

    OCR0A =   OCR0Avalue;    // When TCNT0 = OCR0Avalue+1 a "Compare Match" occurs, 
                             // OC0A is toggled and the timer is cleared. 
    TIMSK0 = _BV(OCIE0A);    // Enable Output Compare Match A Interrupt Enable
}
                                            
//Bit Length Timer                                            
void SetupTimer1()
{
    TIMSK1 &= ~(_BV(OCIE1A));// Disable Output Compare Match A Interrupt

    pinMode (9, OUTPUT);    // Timer 1 "A" output: OC1A (Digital Pin 9)
    digitalWrite(9, LOW);   // Enable Pin 9 and set LOW 

    TCCR1B = _BV(WGM12);     // Wave Generation Mode 2, Clear Timer on Compare (CTC)
                             // Note: different register and bit from 8 Bit Timers
  
    TCCR1A = _BV (COM1A0);  // Toggle OC1A (Digital Pin 9) on Compare Match

    TCCR1B |= _BV (CS12) | _BV (CS10);     // prescaler = 1024

    OCR1AH =   0;
    OCR1AL =   OCR1Avalue;   // When TCNT0 = OCR1Avalue+1 a "Compare Match" occurs, 
                             // OC0A is toggled and the timer is cleared. 
    TIMSK1 = _BV(OCIE1A);    // Enable Output Compare Match A Interrupt Enable
}

char binaryString [binaryBufferSize] = "";

char* convertToBinaryString(String hexString)
{
   char* ptr = binaryString;

   hexString.toUpperCase();
   for (int i =0; i < hexString.length(); i++) {
      char c = hexString.charAt(i);
      if (c >= '0' && c <= '9') {
         int index = (byte) (c - '0');
         strncpy(ptr,hexToBinaryString[index],4);
         ptr = ptr+4;
      } else {
         int index = (byte) (c-'A'+10);
         strncpy(ptr,hexToBinaryString[index],4);
         ptr = ptr+4;
      }
   }
   return binaryString;
}

volatile boolean transmitting = false;   
volatile boolean waiting = false;   
volatile int wait_count = 0;   
            
volatile int repeats = 0;

char* txString;
volatile int index = 0;
int end = 0;
                             
volatile int zero_count =0;
volatile int one_count =0;

void IrTx(String code) {

  txString = convertToBinaryString(code);
  index = 0;
  end = strlen(txString);
  repeats = 0;

  transmitting = true;
  while (transmitting) {
  }

  wait_count = w_count;
  waiting = true;

  while (waiting) {
  }
}

void IrTxSender()  //Called by ISR(TIMER1_COMPA_vect) 
{
    if ((!transmitting)&&(!waiting)) {
       TCCR0A &= ~(_BV (COM0A0));  // Turn off modulation
       return;
    } 
    if (transmitting) {
      if (txString [index] == '1') {
         TCCR0A |= _BV(COM0A0);   // Turn on modulation
      } else {
         TCCR0A &= ~(_BV (COM0A0));  // Turn off modulation
      }
      index++;
      if (index < end ) return;

      index = 0;
      repeats++;
      if (repeats < codeRepeats) return;
      transmitting = false;
      return;
    }
    if (waiting) {
       TCCR0A &= ~(_BV (COM0A0));  // Turn off modulation
       wait_count--;
       if (wait_count <= 0) waiting = false;
    }
}
