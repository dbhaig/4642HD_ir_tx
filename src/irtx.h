/* irtx.h
  Copyright 2015-2016 Don Haig (time4tux at gmail dot com)
  MIT Licence (See LICENSE.txt for details) 
*/

#ifndef ir_tx_H
#define ir_tx_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C"{
#endif

  void SetupTimer0();
  void SetupTimer1();
  void MarkLengthTimeOut();

  void IrTx(String code);
  void IrTxSender();

#ifdef __cplusplus
} // extern "C"
#endif

#endif
