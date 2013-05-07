/*
 * echo - echo characters back to bluetooth device
 *
 * Waits for a connection and then echos each charater received.
 *
 * Debugging is enabled, so if you open the 'Serial Monitor' you can see
 * the search for the HC05 baud and the wait for the BT connection.
 */
#include <Arduino.h>
#include "HC05.h"

#ifdef HC05_SOFTWARE_SERIAL
#include <SoftwareSerial.h>
HC05 btSerial = HC05(A2, A5, A3, A4);  // cmd, state, rx, tx
#else
HC05 btSerial = HC05(3, 2);  // cmd, state
#endif

void setup()
{
  DEBUG_BEGIN(57600);
  btSerial.findBaud();
}

void loop()
{
  btSerial.println("Echo Server- type something");

  while (btSerial.connected())
  {
    if (btSerial.available())
    {
      btSerial.write(btSerial.read());
    }
  }
}
