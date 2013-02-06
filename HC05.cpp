#include <Arduino.h>
#include <HC05.h>

#ifdef DEBUG_SW_PORT
SoftwareSerial DEBUG_SW_PORT;
#endif

#ifdef HC05_SOFTWARE_SERIAL
HC05::HC05(int cmdPin, int statPin, uint8_t rx, uint8_t tx):_btSerial(rx,tx,0)
#else
#define _btSerial HC05_HW_SERIAL_PORT
HC05::HC05(int cmdPin, int statPin)
#endif
{
  pinMode(cmdPin, OUTPUT);
  pinMode(statPin, INPUT);
  _cmdPin = cmdPin;
  _statPin = statPin;
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

/*
 * The down side of this is that the status gets checked for every byte
 * written out which doesn't seem that efficient, but maybe it is
 * correct.
 */
size_t HC05::write(uint8_t byte)
{
  if (digitalRead(_statPin) != HIGH)
  {
      DEBUG_PRINTLN("No Connection");
  }
  _btSerial.write(byte);
}
