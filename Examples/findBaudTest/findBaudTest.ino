/*
 * findBaudTest - Test all supported baud settings
 *
 * The progress and results are printed to Serial, so open the 'Serial
 * Montitor'.
 *
 * The progress and results will be easier to read if you disable the
 * debugging (comment out or delete the "#define DEBUG_HC05" line in
 * HC05.h.
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
  Serial.begin(57600);
  Serial.println("---------- Setup ----------");
  btSerial.findBaud();
  btSerial.setBaud(4800);
  Serial.println("---------- Starting test ----------");
}

void loop()
{
  int numTests = 0;
  int failed = 0;
  unsigned long rate = 0;
  unsigned long rates[] = {4800,9600,19200,38400,57600,115200};

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 6; j++)
    {
      numTests++;
      Serial.print(rates[i]);
      btSerial.setBaud(rates[i]);
      rate = btSerial.findBaud();
      if (rate != rates[i])
      {
        Serial.print(" FAILED: found rate ");
        Serial.println(rate);
        failed++;
      }
      else
      {
        Serial.print("->");
        Serial.print(rates[j]);
        btSerial.setBaud(rates[j]);
        rate = btSerial.findBaud();
        if (rate != rates[j])
        {
          Serial.print("FAILED: found rate ");
          Serial.println(rate);
          failed++;
        }
        else
        {
          Serial.println(" PASSED");
        }
      }
    }
  }

  Serial.println("--------- Tests Complete ----------");
  Serial.print("Results: ");
  Serial.print(failed);
  Serial.print(" of ");
  Serial.print(numTests);
  Serial.println(" tests failed.");

  while (true)
  {
     ;
  }
}
