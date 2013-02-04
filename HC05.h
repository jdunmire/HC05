/*
 * HC05.h - interface definitions for the HC05 library
 */
#ifndef HC05_h
#define HC05_h

#include <SoftwareSerial.h>

class HC05
{
  public:
    HC05(int pin, int rx, int tx);
    unsigned long findBaud();
    int cmd(const char* cmd);

  private:
    int _cmdPin;
    SoftwareSerial _btSerial;
};

#endif
