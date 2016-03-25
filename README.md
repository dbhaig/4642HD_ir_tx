# 4642HD_ir_tx
Arduino UNO program that generates infrared signals to change channels on a CISCO 4642HD cable box

### Hardware

Connect an infrared LED, in series with a 50 ohm resistor, between digital pin 6 and ground on the UNO board.

### Commands

Commands are sent to the Arduino over USB, via the serial interface:
  * Valid commands consist of simple strings terminated by a linefeed "\n" character. 
  * Valid commands are echoed back, once completed. 
  * Invalid commands are ignored 
  * Space characters are treated as delimiters
  * Command strings are limited to 10 characters
  * Once a valid command has been parsed, subsequent characters in the buffer are ignored

| Command                   |   Response                                                                                 |
| ------------------------  | ------------------------------------------------------------------------------------------ |
|?                          | List of valid commands                                                                     |
|IDN?                       | IDN identification string                                                                  |
|POWER                      | "POWER" - Powers on 4642HD                                                                 |
|CHANNEL [0-9][0-9][0-9]    | "CHANNEL XXX" - Where XXX is the channel number specified                                  |
|CHANNEL?                   | "CHANNEL XXX" - Where XXX is the last channel number sent (or 000, if channel is unknown)  |
