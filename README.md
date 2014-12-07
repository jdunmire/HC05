HC05
====
An Arduino library for the HC-05 Bluetooth ITead Studio HC-05 Serial
Port Module.

See the `LICENSE` file for copyright and license information.

The serial port can be configured as any supported Serial port or
a SoftwareSerial port.

Includes a demonstration program that can be uses to change the name
reported by an HC-05 module.

Additional information is available as an
[exercise](http://rockingdlabs.dunmire.org/exercises-experiments/hc05-bluetooth)
at [RockingD Labs](http://rockingdlabs.dunmire.org).


Components
----------
`HC05`
    A class for controlling and communicating through an ITead Studio
    HC-05 Serial Port Module. This class inherits from the Stream class.


###Methods:
The Stream class is extended with the following methods.

`findBaud()`
    Determine HC-05 communications speed. Make this call in setup()
    instead of `begin()`. `begin()` is still avaialble and can be used
    inplace of `findBaud()` if you know the HC-05 communications speed.

`setBaud(unsigned long rate)`
    Specify the HC-05 communications speed. The speed is non-volatile so
    call this only when the rate returned by findBaud() is not the one
    you require.

`setBaud(unsigned long rate, unsigned long parity, unsigned long stopbits)`
    Use this method when you need something besides the default no
    parity, one stop bit settings that are the default.
    __CAUTION!__ The HC-05 supports many serial configurations that are not
    compatible with an Arduino. For example, the Arduino software serial
    port port supports only no parity, one stop bit settings.

`cmd()`
    Send a command to the module. The 'key' (cmdPin) pin is activated to
    put the module in command mode where 'AT' commands are recognized.

`cmdMode2Start(int pwrPin)`
   This is an alternate command mode. This 2nd command mode has the
   advantage forcing the HC-05 into a know communications speed: 38400.
   However, entering this 2nd command mode requires switching the power
   to the HC-05.

`cmdMode2End()`
   Exits the alternate command mode, leaving the power to the HC-05 on.

`connected()` (Only if HC05_STATE_PIN is defined in `HC05.h`)
    Returns true when a BT connection has been established.

`write()`
`print*()`
    The write(), and print*(), methods block until there is a BT
    connection.

Example Programs
----------------
The default library configuration uses a software serial port. The
example programs will work with either a hardware or a software serial
port. The configuration is changed by modifying the `HC05.h` file.

See the `SoftwareSerial.fzz` file for the proper default connections.
(`.fzz` files can be read by the free program available from
[Fritzing](http://fritzing.org/home/))

`changeName`
    This application is one of the reasons I wrote this library. I
    wanted to be able to change the name reported by the HC-05 because I
    have multiple HC-05 modules that I kept mixing up. With this program
    you can set the name of the HC-05 module to reflect something
    physically identifying (or anything else that helps you tell your
    modules apart).

`echo`
    Echo characters as they are received.

`hc05_test`
    Tests the disconnect command (AT+DISC). This was something I used
    during development and probably is not of general interest.

`recover`
    This example used the 2nd command mode to _recover_ the HC-05 when
    its serial port settings are incompatible with the Arduino serial
    ports. Power to HC-05 must be controlled by an Arduino pin.
    See the `Recovery.fzz` diagram for suitable connections.

`findBaudTest`
    Tests both setBaud() and findBaud() by trying every combination of
    supported rates. The output from this example looks best if
    DEBUG_HC05 is not defined. (Simply comment out that line in HC05.h).


Installation
------------
###Option 1: Git (Recommended)
* Follow this [GitHub repository](https://github.com/jdunmire/HC05)
  and use `git` to track your own changes by cloning:

    $ cd ~/sketchbook/libraries
    $ git clone https://github.com/jdunmire/HC05.git

* Start the Arduino IDE and you should find `HC05` in the
  libraries section.

###Option 2: Source only
* Download a ZIP file. The ZIP button at
  [GitHub](https://github.com/jdunmire/HC05) will always get the
  latest version, but you may prefer one of the
  [tagged](https://github.com/jdunmire/HC05/tags) versions.

* Unpack the zip file into your sketchbook library directory
  (`~/sketchbook/libraries` on Linux).

* Rename the resulting directory (or create a symlink) to
  ~/sketchbook/libraries/HC05

* Start the Arduino IDE and you should find `HC05` in the libraries
  section.

### Configuration
By default the library is configured for a software serial port and
debugging output to the hardware serial port (Serial) is turned on. You
will need to edit the HC05.h file if you want to change those settings.

See the `SoftwareSerial.fzz` file for the proper default connections.
The `HardwareSerial.fzz` shows the hardware port alternative. The files
can be read by the free program available from
[Fritzing](http://fritzing.org/home/)

The Bluetooth port is `btSerial` and must be setup as shown at the top
of the Example sketches. If debugging output is enabled in HC05.h (it is
by default) then your sketch must include a DEBUG_BEGIN(baud) command to
initialize the debug output port and set it's baud rate.

#### Hardware Serial Port Issues for UNO
Using the hardware serial port on the UNO comes with some caveats:

  * You will have to disconnect the HC-05 module to upload a sketch.
  * If you use the Arduino `Serial Monitor` you will see the traffic to
    and from the HC-05 serial port. If you type in the `Serial Monitor`
    it will interfere with the HC-05 traffic.

