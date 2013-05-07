/*
 * Change the BT device name
 *
 * This works with a Bluetooth terminal. The user is prompted for a new
 * name which is then written to HC-05.
 *
 */
#include <Arduino.h>
#include "HC05.h"

/*
 * Configure this sketch to work with either a software or a hardware
 * serial port, as configured in HC05.h
 */
#ifdef HC05_SOFTWARE_SERIAL
#include <SoftwareSerial.h>
HC05 btSerial = HC05(A2, A5, A3, A4);  // cmd, state, rx, tx
#else
HC05 btSerial = HC05(3, 2);  // cmd, state
#endif

/*
 * See the ITeadStudio HC-05 datasheet for a full list of commands.
 */
String NewNameCmd("AT+NAME=");


void setup()
{
  btSerial.findBaud();
}


void loop(){
  char buffer[32];
  size_t recvd = 0;
  bool waiting = true;
  String newName;

  btSerial.println("");
  btSerial.print("New name? ");

  // Use a timeout that will give reasonablly quick response to the user.
  btSerial.setTimeout(100);
  while (waiting)
  {
    if (btSerial.available())
    {
      recvd = btSerial.readBytes(buffer, 32);
      for (size_t i = 0; i < recvd; i++)
      {
        if (buffer[i] != '\n')
        {
          newName += buffer[i];
          btSerial.print(buffer[i]);
        }
        else
        {
          btSerial.println(" ");
          waiting = false;
          break;
        }
      }
    }
    delay(100);
  }

  newName.toCharArray(buffer, 32);
  newName = NewNameCmd + newName;
  newName.toCharArray(buffer, 32);

  // make sure there is no pending output to interfere with commands
  btSerial.flush();

  // The name change command takes extra time.
  // 1000ms is large enough, but arbitrary.
  if (btSerial.cmd(buffer,1000))
  {
    btSerial.println("Name changed.");
    btSerial.println("Reconnect or rescan to see the result.");
    btSerial.println("Disconnecting...");
    btSerial.flush();
    btSerial.cmd("AT+DISC", 1000);
  }
  else
  {
    btSerial.println("Name NOT changed.");
  }
  
  // Send a count to the port just to indicate activity.
  // This will appear after the connection is re-established, or
  // immediately if the name change command fails.
  for (uint8_t i = 0; true; i++)
  {
    if (i == 0)
    {
      btSerial.println("");
    }
    btSerial.print(i);
    btSerial.print('\r');
    delay(1000);
  }
}

