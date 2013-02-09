#include <Arduino.h>
#include "HC05.h"
#include <SoftwareSerial.h>

#ifdef HC05_SOFTWARE_SERIAL
HC05 myHC05 = HC05(3, 2, 4, 5);  // cmd, state, rx, tx
#else
HC05 myHC05 = HC05(3, 2);  // cmd, state
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
  myHC05.println(myHC05.findBaud());
  
}

void loop()
{
  if (myHC05.connected()){
    DEBUG_PRINTLN("Connected");
    myHC05.cmd("AT+DISC", 1000);
  }
  else
  {
    DEBUG_PRINTLN("Disconnected");
  }
  delay(1000);
}
