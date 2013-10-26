HC05
====
An Arduino library for the HC-05 Bluetooth ITead Studio HC-05 Serial
Port Module.

See the `LICENSE` file for copyright and license information.

The serial port can be configured as any supported Serial port or
a SoftwareSerial port.

Includes a demonstration program that can be uses to change the name
reported by an HC05 module.

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
    Determine HC-05 communications speed. Make this call in setup().

`setBaud()`
    Specify the HC-05 communications speed. The speed is non-volatile so
    call this only when the rate returned by findBaud() is not the one
    you require.

`cmd()`
    Send a command to the module. The 'key' (cmdPin) pin is activated to
    put the module in command mode where 'AT' commands are recognized.

`connected()`
    Returns true when a BT connection has been established. (Declared only if HC05_STATE_PIN defined)

`write()`
    The write(), and print*(), methods block until there is a BT
    connection.

Example Programs
----------------
`changeName`
    This application is one of the reasons I wrote this library. I
    wanted to be able to change the name reported by the HC05 because I
    have multiple HC05 modules that I kept mixing up. With this program
    you can set the name of the HC05 module to reflect something
    physically identifying (or anything else that helps you tell your
    modules apart).

`echo`
    Echo characters as they are received.

`hc05_test`
    Tests the disconnect command (AT+DISC). This was something I used
    during development and probably is not of general interest.


Installation
------------
###Option 1: Source only
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

###Option 2: Git (Recommended)
* Follow this [GitHub repository](https://github.com/jdunmire/HC05)
  and use `git` to track your own changes by cloning:

    $ cd ~/sketchbook/libraries  
    $ git clone https://github.com/jdunmire/HC05.git

* Start the Arduino IDE and you should find `HC05` in the
  libraries section.

By default the library is configured for a software serial port and
debugging output to the hardware serial port (Serial) is turned on. You
will need to edit the HC05.h file if you want to change those settings.

The Bluetooth port is `btSerial` and must be setup as shown at the top
of the Example sketches. If debugging output is enabled in HC05.h (it is
by default) then your sketch must include a DEBUG_BEGIN(baud) command to
initialize the debug output port and set it's baud rate.

