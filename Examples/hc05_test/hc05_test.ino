#include "HC05.h"
#include <SoftwareSerial.h>

HC05 myHC05 = HC05(3, 4, 5);

void setup()
{
  Serial.begin(9600);
  Serial.write("Starting");
  delay(1000);
  Serial.println(myHC05.findBaud());

}

void loop()
{
  delay(1000);
  Serial.write("Starting loop\n");
  Serial.println(myHC05.cmd("AT"));
  delay(1000);
  return;
}
