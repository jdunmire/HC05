#include <Arduino.h>
#include <HC05.h>

#ifdef DEBUG_SW_PORT
SoftwareSerial DEBUG_SW_PORT;
#endif

#ifdef HC05_SOFTWARE_SERIAL
HC05::HC05(int cmdPin, int statePin, uint8_t rx, uint8_t tx):_btSerial(rx,tx,0)
#else
#define _btSerial HC05_HW_SERIAL_PORT
HC05::HC05(int cmdPin, int statePin)
#endif
{
  pinMode(cmdPin, OUTPUT);
  pinMode(statePin, INPUT);
  _cmdPin = cmdPin;
  _statePin = statePin;
  _bufsize = sizeof(_buffer)/sizeof(char);
}

static const unsigned long rates[] = {9600,19200,57600,115200,38400};

unsigned long HC05::findBaud()
{
  const int bt_rx = 4;
  const int bt_tx = 5;
  int numRates = sizeof(rates)/sizeof(unsigned long);
  int response = false;
  int recvd = 0;
  //char _buffer[128];

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
    recvd = _btSerial.readBytes(_buffer,_bufsize);
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

int HC05::cmd(const char* cmd, unsigned long timeout)
{
  int recvd = 0;
  DEBUG_PRINTLN(cmd);

  digitalWrite(_cmdPin, HIGH);
  // No spec for how long it takes to enter command mode, but 100ms
  // seems to work- assuming the output has been drained.
  delay(100);
  _btSerial.write(cmd);
  _btSerial.write("\r\n");
  _btSerial.setTimeout(timeout);
  do {
      // ATTENTION: At least through Arduino v1.0.3, it is not possible
      //            to tell the difference between a timeout and
      //            receiving only the termination character (NL in this
      //            case), because the termination character is not
      //            returned and timeout is not returned as a unique
      //            indication.
      //            In this case the result would be an early return
      //            of a multiline response before the OK is received.
      //            The return would incorrectly indicate an error (no
      //            OK response).
      recvd = _btSerial.readBytesUntil('\n',_buffer,_bufsize);
      if (recvd > 0)
      {
          DEBUG_WRITE((uint8_t *)_buffer,recvd);
          DEBUG_WRITE('\n');
      }
      else
      {
          DEBUG_PRINTLN("timeout");
      }
  }
  while ((recvd > 0) && (_buffer[0] != 'O' || _buffer[1] != 'K'));

  digitalWrite(_cmdPin, LOW);

  // Emperically determined that it takes some time to reliablly exit
  // command mode. The appeared to be a baud rate dependency and with
  // >100ms required at 9600 baud.
  delay(150);
  return((_buffer[0] == 'O' && _buffer[1] == 'K'));
}


/*
 * If setBaud() is called while the HC-05 is connected, then
 * it will be disconnected when the AT+RESET command is issued, and
 * it may take 2 (or more?) connection attempts to reconnect. The extra
 * connect attempts may be a host side issue and not specific to the
 * HC-05 module.
 */
void HC05::setBaud(unsigned long baud)
{
  int recvd = 0;
  digitalWrite(_cmdPin, HIGH);
  delay(200);
  DEBUG_WRITE("AT+UART=");
  _btSerial.write("AT+UART=");
  DEBUG_PRINT(baud);
  _btSerial.print(baud);
  DEBUG_WRITE(",0,0\r\n");
  _btSerial.write(",0,0\r\n");
  recvd = _btSerial.readBytes(_buffer,_bufsize);
  if (recvd > 0)
  {
      DEBUG_WRITE((uint8_t *)_buffer,recvd);
  }
  else
  {
      DEBUG_PRINTLN("timeout");
  }
  digitalWrite(_cmdPin, LOW);
  delay(100);
  cmd("AT+RESET");
  _btSerial.begin(baud);
  delay(1000);
}

int HC05::available()
{
    _btSerial.available();
}

int HC05::peek()
{
    _btSerial.peek();
}

void HC05::flush()
{
    _btSerial.flush();
}

int HC05::read()
{
    _btSerial.read();
}

bool HC05::connected()
{
    return(digitalRead(_statePin)?true:false);
}

size_t HC05::write(uint8_t byte)
{
  // The down side of this check is that the status gets checked for
  // every byte written out which doesn't seem that efficient.
  if (digitalRead(_statePin) != HIGH)
  {
      DEBUG_PRINT("No Connection, waiting...");
      while (digitalRead(_statePin) == LOW)
      {
          delay(100);
      }
      DEBUG_PRINTLN("OK");
  }
  _btSerial.write(byte);
}
