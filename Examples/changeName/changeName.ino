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
HC05 myHC05 = HC05(3, 2, 4, 5);  // cmd, state, rx, tx
#else
HC05 myHC05 = HC05(3, 2);  // cmd, state
#endif

/*
 * See the ITeadStudio HC-05 datasheet for a full list of commands.
 */
String NewNameCmd("AT+NAME=");


void setup()
{
  myHC05.findBaud();
}


void loop(){
  char buffer[32];
  size_t recvd = 0;
  bool waiting = true;
  String newName;

  myHC05.println("");
  myHC05.print("New name? ");

  // Use a timeout that will give reasonablly quick response to the user.
  myHC05.setTimeout(100);
  while (waiting)
  {
    if (myHC05.available())
    {
      recvd = myHC05.readBytes(buffer, 32);
      for (size_t i = 0; i < recvd; i++)
      {
        if (buffer[i] != '\n')
        {
          newName += buffer[i];
          myHC05.print(buffer[i]);
        }
        else
        {
          myHC05.println(" ");
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
  myHC05.flush();

  // The name change command takes extra time.
  // 1000ms is large enough, but arbitrary.
  if (myHC05.cmd(buffer,1000))
  {
    myHC05.println("Name changed.");
    myHC05.println("Reconnect or rescan to see the result.");
    myHC05.println("Disconnecting...");
    myHC05.flush();
    myHC05.cmd("AT+DISC", 1000);
  }
  else
  {
    myHC05.println("Name NOT changed.");
  }
  
  // Send a count to the port just to indicate activity.
  // This will appear after the connection is re-established, or
  // immediately if the name change command fails.
  for (uint8_t i = 0; true; i++)
  {
    if (i == 0)
    {
      myHC05.println("");
    }
    myHC05.print(i);
    myHC05.print('\r');
    delay(1000);
  }
}

