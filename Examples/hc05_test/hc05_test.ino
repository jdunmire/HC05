#include "HC05.h"
#include <SoftwareSerial.h>

HC05 myHC05 = HC05(3, 4, 5);

void setup()
{
  Serial.begin(9600);
  Serial.write("Starting");
  delay(3000);
  Serial.println(myHC05.findBaud());
  Serial.write("AT #1 ");
  Serial.println(myHC05.cmd("AT"));
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

}

void loop()
{
  delay(1000);
  Serial.write("Starting loop\n");
  for(int i = 0; i<3; i++)
  {
    Serial.println(myHC05.cmd("AT"));
    delay(1000);
  }
  Serial.write("now reset");
  myHC05.cmd("AT+RESET");
}
