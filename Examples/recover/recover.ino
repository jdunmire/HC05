/*
 * recover - recover from forgotten serial settings
 *
 * Uses cmdMode2 to set the HC05 to 19200 baud, no parity, ones stop bit
 * (19200N1). 19200N1 is supported by both the hardware and software
 * serial ports on the Arduino.
 *
 * Debugging is enabled, so if you open the 'Serial Monitor' you can see
 * the process. Note that if you don't get the 'Serial Monitor' open
 * before the 'AT+UART=' command is issued, you will not really see the
 * old settings. There is a three second delay at the beginning to give
 * you a chance to get the serial monitor running.
 *
 * The `Serial Monitor` should be set to 57600 baud.
 *
 */
#include <Arduino.h>
#include "HC05.h"

#ifdef HC05_SOFTWARE_SERIAL
#include <SoftwareSerial.h>
HC05 btSerial = HC05(A2, A5, A3, A4);  // cmd, state, rx, tx
#else
HC05 btSerial = HC05(3, 2);  // cmd, state
#endif

int powerPin = 7;

void setup()
{
  DEBUG_BEGIN(57600);
  btSerial.cmdMode2Start(powerPin);

  // Provide some time for the user to start the serial monitor
  delay(3000);

  // For curiosity's sake, ask for the old settings
  btSerial.cmd("AT+UART?");

  // Now set the baud to 19200N1
  btSerial.cmd("AT+UART=19200,0,0");

  // Exit command mode and switch to the new baud setting
  btSerial.cmd("AT+RESET");
  btSerial.cmdMode2End();
}


void loop()
{
  int c;
  unsigned long rate;
  rate = btSerial.findBaud();

  DEBUG_PRINTLN("");

  if (rate == 19200)
  {
    DEBUG_PRINTLN("Recovery successful. HC05 serial settings = 19200N1");
  }
  else
  {
    DEBUG_PRINT("Recovery failed. Did you connect HC05 power to pin ");
    DEBUG_PRINT(powerPin);
    DEBUG_PRINTLN("?");
  }
  while (true)
  {
    ;
  }
}
