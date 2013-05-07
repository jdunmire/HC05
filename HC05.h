/*
 * HC05.h - interface definitions for the HC05 library
 *
 * Select hardware or software serial port:
 *   Define HC05_SOFTWARE_SERIAL to select a SoftwareSerial port, then
 *   initialize the HC05 class with either two arguments, for a hardware port,
 *   or five arguments for a softare serial port:
 *
 *     HC05(cmdPin, statePin)
 *     or
 *     HC05(cmdPin, statePin, rxPin, txPin)
 *     
 * Specify an alternate hardware serial port by changing the
 * HC05_HW_SERIAL_PORT define.
 *
 * Define DEBUG_HC05 to enable debugging messages and use the
 * DEBUG_BEGIN() macro in the sketch setup() function.
 * By default, debugging messages go to the harware serial port, Serial.
 * Change that by defining DEBUG_SW_PORT.
 *
 */
#ifndef HC05_h
#define HC05_h

#include <inttypes.h>
#include <Stream.h>
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

class HC05 : public Stream
{
  public:
    HC05(int cmdPin, int statePin);
    HC05(int cmdPin, int statePin, uint8_t rx, uint8_t tx);
    unsigned long findBaud();
   
    // cmd(): 100ms default timeout covers simple commands, but commands
    // that manage the connection are likely to take much longer.
    int cmd(const char* cmd, unsigned long timeout=100);

    void setBaud(unsigned long baud);
    bool connected(void);

    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    using Print::write;
#ifdef HC05_SOFTWARE_SERIAL
    SoftwareSerial _btSerial;
#endif

  private:
    int _cmdPin;
    int _statePin;
    int _bufsize;
    char _buffer[32];
};

extern HC05 btSerial;
#endif
