/*
 * recover - recover from forgotten serial settings
 *
 * Uses a power-cycle with the cmd pin held high to get into command
 * mode with the serial port set to 38400 baud, no parity, one stop bit,
 * and then sets the HC05 to 19200 baud, no parity, one stop bit,
 * (19200N1). 19200N1 is supported by both the hardware and software
 * serial ports on the Arduino.
 *
 * Debugging is enabled, so if you open the 'Serial Monitor' you can see
 * the process. Note that if you don't get the 'Serial Monitor' open
 * before the 'AT+UART=' command is issued, you will not really see the
 * old settings.
 *
 * The `Serial Monitor` should be set to 57600 baud.
 *
 * All the real work happens in setup(), so an echo server is run by
 * loop(). Similar to the echo.ino example.
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
int cmdPin = 6;

void setup()
{
  DEBUG_BEGIN(57600);
  // Turn off power to the HC05
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
  delay(500);

  // Set the command pin high and apply power
  pinMode(cmdPin, OUTPUT);
  digitalWrite(cmdPin, HIGH);
  digitalWrite(powerPin, HIGH);

  // Allow time for the HC05 to initialize
  delay(1000);

  // Set the Arduino serial port to 38400 to match
  // the HC05 command mode
  btSerial.begin(38400);

  // For curiosity's sake, ask for the old settings
  btSerial.cmd("AT+UART?");

  // Now set the baud to 19200N1
  btSerial.cmd("AT+UART=19200,0,0");

  // Exit command mode and switch to the new baud setting
  digitalWrite(cmdPin, LOW);
  btSerial.cmd("AT+RESET");
  delay(500);

  // Set the Arduino serial port to match
  // the rate we set above.
#ifdef HC05_SOFTWARE_SERIAL
  btSerial.begin(19200);
#else
  btSerial.begin(19200, SERIAL_8N1);
#endif
}


void loop()
{
  int c;
  btSerial.println("Echo Server- type something");
  DEBUG_PRINTLN("Echo server running...");

  while (btSerial.connected())
  {
    if (btSerial.available())
    {
      c = btSerial.read();
      DEBUG_WRITE(c);
      btSerial.write(c);
    }
  }
}
