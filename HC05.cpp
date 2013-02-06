#include <Arduino.h>
#include <HC05.h>

#ifdef DEBUG_SW_PORT
SoftwareSerial DEBUG_SW_PORT;
#endif

#ifdef HC05_SOFTWARE_SERIAL
HC05::HC05(int pin, int rx, int tx):_btSerial(rx,tx)
#else
#define _btSerial HC05_HW_SERIAL_PORT
HC05::HC05(int pin)
#endif
{
  pinMode(pin, OUTPUT);
  _cmdPin = pin;
  pinMode(2, INPUT);
}


static const unsigned long rates[] = {9600,19200,57600,115200,38400};

unsigned long HC05::findBaud()
{
  const int bt_rx = 4;
  const int bt_tx = 5;
  int numRates = sizeof(rates)/sizeof(unsigned long);
  int response = false;
  int recvd = 0;
  char buffer[32];

  DEBUG_PRINTLN("findBaud");
  digitalWrite(_cmdPin, HIGH);
  delay(100);
  for(int rn = 0; rn < numRates; rn++)
  {
    _btSerial.begin(rates[rn]);
    _btSerial.setTimeout(100);
    DEBUG_WRITE("Trying ");
    DEBUG_PRINT(rates[rn]);
    DEBUG_WRITE("... ");
    _btSerial.write("AT\r\n");
    recvd = _btSerial.readBytes(buffer,32);
    if (recvd > 0)
    {
      DEBUG_PRINTLN("Found.");
      // FIXME: refactor to a single return
      digitalWrite(_cmdPin, LOW);
      return(rates[rn]);
    }
    else
    {
        DEBUG_PRINTLN("x");
    }
  }
  digitalWrite(_cmdPin, LOW);
  DEBUG_WRITE("\r\nNo connection\r\n");
  return(0);
}

int HC05::cmd(const char* cmd)
{
  int recvd = 0;
  char buffer[32];
  DEBUG_PRINTLN(cmd);
  digitalWrite(_cmdPin, HIGH);
  delay(100);
  _btSerial.write(cmd);
  _btSerial.write("\r\n");
  _btSerial.setTimeout(100);
  do {
      recvd = _btSerial.readBytesUntil('\n',buffer,32);
      if (recvd > 0)
      {
          DEBUG_PRINTLN(recvd);
          DEBUG_PRINTLN((unsigned char)(buffer[recvd-1]),HEX);
          DEBUG_WRITE((uint8_t *)buffer,recvd);
      }
  } while ((recvd > 0) && (buffer[0] != 'O' || buffer[1] != 'K'));
  //recvd = _btSerial.readBytes(buffer,32);
  digitalWrite(_cmdPin, LOW);
  return((buffer[0] == 'O' && buffer[1] == 'K'));
}

void HC05::setBaud(unsigned long baud)
{
  int recvd = 0;
  char buffer[32];
  digitalWrite(_cmdPin, HIGH);
  delay(200);
  DEBUG_WRITE("AT+UART=");
  _btSerial.write("AT+UART=");
  DEBUG_PRINT(baud);
  _btSerial.print(baud);
  DEBUG_WRITE(",0,0\r\n");
  _btSerial.write(",0,0\r\n");
  recvd = _btSerial.readBytes(buffer,32);
  digitalWrite(_cmdPin, LOW);
  delay(100);
  DEBUG_WRITE((uint8_t *)buffer,recvd);
  cmd("AT+RESET");
  _btSerial.begin(baud);
  delay(1000);
}

void HC05::write(const char* buffer)
{
  if (digitalRead(2) != HIGH)
  {
      DEBUG_PRINTLN("No Connection");
  }
  else
  {
      DEBUG_WRITE(buffer);
      _btSerial.write(buffer);
  }
}
