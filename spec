Copyright 2015-2016 Don Haig (time 4 tux at gmail dot com)

Spec for IR Transmitter
=======================

The IR transmitter will generate transmissions that will control a CISCO HD4642 cable box
in response to command strings received via a serial port.

Commands
--------
Commands consist of simple strings terminated by a linefeed "\n" character.
Command is echoed back, via the serial interface, once it is completed. 

Invalid commands are ignored.
Space characters are treated as delimiters
Command strings are limited to 10 characters
Once a valid command has been parsed, subsequent characters are ignored

Valid commands 
--------------
Command                 Response
=======                 ========
?                       <Outputs help menu>
IDN?                    "IDN <Identification String>"
POWER                   "POWER"
CHANNEL [0-9][0-9][0-9] "CHANNEL XXX" - Where XXX is the channel number specified 
CHANNEL?                "CHANNEL XXX" - Where XXX is the last channel number sent (or 000, if channel is unknown)


Software Routines
-----------------

SerialInput - ISR that continuously monitors serial input for incoming characters:
    If CharacterDetected
         If CharacterIsLinefeed
             CopyInputBufferToParseBuffer
             ClearInputBuffer
             SetCommandReceived
         Else
             If InputBufferFull
                ClearInputBuffer
             SaveCharacterToInputBuffer
                              
Parser - Called in main loop to process incoming commands from serial input
    If CommandReceived
        CompareParseBufferWithValidCommandStrings:
            IdnRequested
                EchoIdnResponseToSerialPort
            LastChannelRequested
                EchoLastChannelToSerialPort
            OnOffCommand
                TransmitOnOffSequence
                EchoParseStringToSerialPort
            ValidDigits
                TransmitDigits
                SaveDigitsAsLastChannel
                EchoParseStringToSerialPort
        ClearCommandReceived

TransmitOnOffCode()
  ConvertHexDescriptionToMarkDurationsAndSpaceDurations
    For Number of Repeats
      TransmitMarksAndSpacesSeries 

TransmitChannelCodes(Digits)
   For each Digit
      TransmitButtonCode(digit, repeats)
        For each ButtonCode
          ConvertHexDescriptionToMarkDurationsAndSpaceDurations
            For Number of Repeats
              TransmitMarksAndSpacesSeries 
