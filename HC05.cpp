#include <Arduino.h>
#include <HC05.h>
#include <SoftwareSerial.h>

HC05::HC05(int pin, int rx, int tx):_btSerial(rx,tx)
{
  pinMode(pin, OUTPUT);
  _cmdPin = pin;
}

static const unsigned long rates[] = {9600,19200,57600,115200,38400};

unsigned long HC05::findBaud()
{
  const int bt_rx = 4;
  const int bt_tx = 5;
  int numRates = sizeof(rates)/sizeof(unsigned long);
  int response = false;
  int recvd = 0;
  char buffer[16];

  digitalWrite(_cmdPin, HIGH);
  Serial.begin(9600);

  for(int rn = 0; rn < numRates; rn++)
  {
    _btSerial.begin(rates[rn]);
    _btSerial.setTimeout(100);
    Serial.write("\r\nTrying ");
    Serial.print(rn);
    Serial.write(", ");
    Serial.print(rates[rn]);
    _btSerial.write("AT\r\n");
    recvd = _btSerial.readBytes(buffer,16);
    if (recvd > 0)
    {
      Serial.write("Found\r\n");
      Serial.write((uint8_t *)buffer,recvd);
      // FIXME: refactor to a single return
      digitalWrite(_cmdPin, LOW);
      return(rates[rn]);
    }
  }
  Serial.write("\r\nNo connection\r\n");
  digitalWrite(_cmdPin, LOW);
  return(0);
}

int HC05::cmd(const char* cmd)
{
  int recvd = 0;
  char buffer[16];
  digitalWrite(_cmdPin, HIGH);
  _btSerial.write(cmd);
  _btSerial.write("\r\n");
  _btSerial.setTimeout(100);
  recvd = _btSerial.readBytes(buffer,16);
  Serial.println(recvd);
  Serial.write((uint8_t *)buffer,recvd);
  digitalWrite(_cmdPin, LOW);
  return((buffer[0] == 'O' && buffer[1] == 'K'));
}

