/*
 * HC05.h - interface definitions for the HC05 library
 */
#ifndef HC05_h
#define HC05_h

#include <SoftwareSerial.h>

#define DEBUG_HC05
#ifdef DEBUG_HC05
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
  #define DEBUG_WRITE(...) Serial.write(__VA_ARGS__)
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
  #define DEBUG_BEGIN(baud)
  #define DEBUG_WRITE(...)
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)
#endif
class HC05
{
  public:
    HC05(int pin, int rx, int tx);
    unsigned long findBaud();
    int cmd(const char* cmd);
#ifdef DEBUG_HC05
    SoftwareSerial _btSerial;
#endif

  private:
    int _cmdPin;
};

#endif
