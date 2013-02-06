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
  DEBUG_BEGIN(19200);
  DEBUG_PRINTLN("Setup");
  delay(3000);  // this delay is for debugging convenience only
  DEBUG_PRINTLN("DelayComplete");
  myHC05.findBaud();
}

void loop()
{
  myHC05.setBaud(19200);
  myHC05.cmd("AT+STATE?");
  myHC05.println("19200");

  delay(1000); // Allow buffer to drain
  myHC05.setBaud(38400);
  myHC05.cmd("AT+STATE?");
  myHC05.println("38400");

  delay(1000); // Allow buffer to drain
  myHC05.setBaud(57600);
  myHC05.cmd("AT+STATE?");
  myHC05.println("57600");
  /*
   * UNO does not reliably read from HC-05 at 115.2k
   */
  //delay(1000); // Allow buffer to drain
  //myHC05.setBaud(115200);
  //myHC05.cmd("AT+STATE?");
  //myHC05.println("115200");
  
  for(int i = 0; i<10; i++)
  {
    //myHC05.cmd("AT");
    myHC05.write("got here\r\n");
    myHC05.println(i);
    delay(1000);
  }
  for (int i = 0; true; i++)
  {
    myHC05.cmd("AT");
    delay(1000);
    myHC05.write("got here 2\r\n");
    delay(1000);
  }
//  myHC05.setBaud(19200);
//  for(int i = 0; i<3; i++)
 // {
  //  myHC05.cmd("AT");
   // delay(1000);
 // }
}
