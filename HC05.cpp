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
    _cmdPin = cmdPin;
    cmdMode = false;
#ifdef HC05_STATE_PIN
    pinMode(statePin, INPUT);
    _statePin = statePin;
#endif
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
    setCmdPin(HIGH);
    delay(100);
    for(int rn = 0; rn < numRates; rn++)
    {
        _btSerial.begin(rates[rn]);
        _btSerial.setTimeout(100);
        _btSerial.flush();
        DEBUG_WRITE("Trying ");
        DEBUG_PRINT(rates[rn]);
        DEBUG_WRITE("... ");
        _btSerial.write("AT\r\n");
        recvd = _btSerial.readBytes(_buffer,_bufsize);
        if (recvd > 0)
        {
            DEBUG_PRINTLN("Found.");
            // FIXME: refactor to a single return
            setCmdPin(LOW);
            return(rates[rn]);
        }
        else
        {
            DEBUG_PRINTLN("x");
        }
    }
    setCmdPin(LOW);
    DEBUG_WRITE("\r\nNo connection\r\n");
    return(0);
}

int HC05::cmd(const char* cmd, unsigned long timeout)
{
    int recvd = 0;
    DEBUG_PRINTLN(cmd);

    setCmdPin(HIGH);
    // No spec for how long it takes to enter command mode, but 100ms
    // seems to work- assuming the output has been drained.
    delay(100);
    _btSerial.write(cmd);
    _btSerial.write("\r\n");
    _btSerial.setTimeout(timeout);
    do
    {
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

    setCmdPin(LOW);

    // Empirically determined that it takes some time to reliably exit
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
    setCmdPin(HIGH);
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
    setCmdPin(LOW);
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

void HC05::begin(unsigned long baud)
{
    _btSerial.begin(baud);
}

#ifndef HC05_SOFTWARE_SERIAL
// only hardware serial ports support parity/stop bit configuration
void HC05::begin(unsigned long baud, uint8_t config)
{
    _btSerial.begin(baud, config);
}
#endif

#ifdef HC05_STATE_PIN
bool HC05::connected()
{
    return(digitalRead(_statePin)?true:false);
}
#endif

size_t HC05::write(uint8_t byte)
{
#ifdef HC05_STATE_PIN
    // The down side of this check is that the status gets checked for
    // every byte written out. That doesn't seem efficient.
    if (digitalRead(_statePin) != HIGH)
    {
        DEBUG_PRINT("No Connection, waiting...");
        while (digitalRead(_statePin) == LOW)
        {
            delay(100);
        }
        DEBUG_PRINTLN("OK");
    }
#endif
    _btSerial.write(byte);
}


void HC05::cmdMode2Start(int pwrPin)
{
    pinMode(pwrPin, OUTPUT);
    digitalWrite(pwrPin, LOW);
    delay(200);  // off or reset time
    digitalWrite(_cmdPin, HIGH);
    digitalWrite(pwrPin, HIGH);
    cmdMode = true;
    _btSerial.begin(38400);
    delay(1000);  // time for the HC05 to initialize
}


void HC05::cmdMode2End(void)
{
    digitalWrite(_cmdPin, LOW);
    cmdMode = false;
    delay(1000);
}

void HC05::setCmdPin(bool state)
{
    if (cmdMode == false)
    {
        digitalWrite(_cmdPin, state);
    }
}
