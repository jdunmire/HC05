HC05
====
An Arduino library for the HC-05 Bluetooth ITead Studio HC-05 Serial
Port Module.

See the `LICENSE` file for copyright and license information.

The serial port can be configured as any supported Serial port or
a SoftwareSerial port.


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
    Returns true when a BT connection has been established.

`write()`
    The write(), and print*(), methods block until there is a BT
    connection.


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

