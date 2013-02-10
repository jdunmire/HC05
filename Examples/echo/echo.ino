#include <Arduino.h>
#include "HC05.h"
#include <SoftwareSerial.h>

#ifdef HC05_SOFTWARE_SERIAL
HC05 myHC05 = HC05(3, 2, 4, 5);  // cmd, state, rx, tx
#else
HC05 myHC05 = HC05(3, 2);  // cmd, state
#endif

void setup()
{
  myHC05.findBaud();
}

void loop()
{
  myHC05.println("Echo Server- type something");

  while (myHC05.connected())
  {
    if (myHC05.available())
    {
      myHC05.write(myHC05.read());
    }
  }
}
