/*
 * hc05_test - Test the disconnect command
 *
 * When the main loop detects that a bluetooth device is connected it
 * disconnects the device.
 *
 * Montor the progress using the debug port.
 */
#include <Arduino.h>
#include "HC05.h"

#ifdef HC05_SOFTWARE_SERIAL
#include <SoftwareSerial.h>
HC05 btSerial = HC05(A2, A5, A3, A4);  // cmd, state, rx, tx
#else
HC05 btSerial = HC05(3, 2);  // cmd, state
#endif

#ifdef DEBUG_HC05
#ifdef DEBUG_SW_PORT
  extern SoftwareSerial DEBUG_PORT;
#endif
#endif

void setup()
{
  DEBUG_BEGIN(57600);
  DEBUG_PRINTLN("Setup");
  delay(3000);  // this delay is for debugging convenience only
  DEBUG_PRINTLN("DelayComplete");
  btSerial.println(btSerial.findBaud());
  
}

void loop()
{
  if (btSerial.connected()){
    DEBUG_PRINTLN("Connected");
    btSerial.cmd("AT+DISC", 1000);
  }
  else
  {
    DEBUG_PRINTLN("Disconnected");
  }
  delay(1000);
}
