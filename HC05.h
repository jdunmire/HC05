/*
 * HC05.h - interface definitions for the HC05 library
 *
 * Select hardware or software serial port:
 *   Define HC05_SOFTWARE_SERIAL to select a SoftwareSerial port, then
 *   initialize the HC05 class with three arguments:
 *     HC05(cmdPin, rxPin, txPin)
 * or
 *   Specify an alternate hardware serial port by changing the
 *   HC05_HW_SERIAL_PORT define.
 *
 * Define DEBUG_HC05 to enable debugging messages.
 * By default, debugging messages go to the harware serial port, Serial.
 * Change that by defining DEBUG_SW_PORT.
 *
 */
#ifndef HC05_h
#define HC05_h

#include <SoftwareSerial.h>

// This macro must be defined
#define HC05_HW_SERIAL_PORT Serial

/*
 * Optional macros, define as needed
 */
#define HC05_SOFTWARE_SERIAL
#define DEBUG_HC05
//#define DEBUG_SW_PORT swserial(4,5)

#ifdef DEBUG_HC05
  #ifdef DEBUG_SW_PORT
  #define DEBUG_PORT swserial
  #else
  #define DEBUG_PORT Serial
  #endif

  #define DEBUG_BEGIN(baud) DEBUG_PORT.begin(baud)
  #define DEBUG_WRITE(...) DEBUG_PORT.write(__VA_ARGS__)
  #define DEBUG_PRINT(...) DEBUG_PORT.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) DEBUG_PORT.println(__VA_ARGS__)

#else
  #define DEBUG_BEGIN(baud)
  #define DEBUG_WRITE(...)
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)

#endif  // DEBUG_HC05

class HC05 : public Print
{
  public:
    HC05(int cmdPin, int statPin);
    HC05(int cmdPin, int statPin, uint8_t rx, uint8_t tx);
    unsigned long findBaud();
    int cmd(const char* cmd);
    void setBaud(unsigned long baud);
    virtual size_t write(uint8_t byte);
    using Print::write;
#ifdef HC05_SOFTWARE_SERIAL
    SoftwareSerial _btSerial;
#endif

  private:
    int _cmdPin;
    int _statPin;
};

#endif
