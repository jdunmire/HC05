#include <Arduino.h>
#include "HC05.h"
#include <SoftwareSerial.h>

#ifdef HC05_SOFTWARE_SERIAL
HC05 myHC05 = HC05(3, 4, 5);
#else
HC05 myHC05 = HC05(3);
#endif

#ifdef DEBUG_HC05
#ifdef DEBUG_SW_PORT
  extern SoftwareSerial DEBUG_PORT;
#endif
#endif

void setup()
{
  DEBUG_BEGIN(9600);
  DEBUG_PRINTLN("Setup");
  delay(3000);
  DEBUG_PRINTLN("DelayComplete");
  myHC05.findBaud();
  myHC05.cmd("AT");
  /*
  Serial.write("Setting baud");
  myHC05.cmd("AT+UART=9600,0,0");
  delay(100);
  Serial.write("now reset 1 ");
  myHC05.cmd("AT+RESET");
  //digitalWrite(5, LOW);
  delay(1000);
  //digitalWrite(5, HIGH);
  Serial.write("AT #2 ");
  Serial.println(myHC05.cmd("AT"));
  Serial.write("New Baud 9600??");
  Serial.println(myHC05.findBaud());
  */

}

void loop()
{
  myHC05.setBaud(19200);
  myHC05.cmd("AT+STATE?");
  delay(1000);
  myHC05.cmd("AT+STATE?");
  /*
  delay(1000);
  myHC05.cmd("AT+STATE?");
  delay(1000);
  myHC05.cmd("AT+STATE?");
  delay(1000);
  myHC05.cmd("AT+STATE?");
  delay(1000);
  myHC05.cmd("AT+STATE?");
  delay(1000);
  myHC05.cmd("AT+STATE?");
  delay(1000);
  myHC05.cmd("AT+STATE?");
  delay(1000);
  myHC05.cmd("AT+STATE?");
  delay(1000);
  myHC05.write("9600\r\n");
  delay(1000);
/*  myHC05.setBaud(19200);
  myHC05.write("19200\r\n");
  myHC05.setBaud(38400);
  myHC05.write("38400\r\n");
  myHC05.setBaud(57600);
  myHC05.write("57600\r\n");
  //myHC05.setBaud(115200);
  //myHC05.write("115200\r\n");
  */
  delay(1000);
  for(int i = 0; i<10; i++)
  {
    //myHC05.cmd("AT");
    myHC05.write("got here\r\n");
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
