# nodatron-ino

This project contains the code that will be compiled and uploaded to the Arduino.

Helpful command-line tool if you do not want to use the GUI (Python 2.6+ required)
git clone git://github.com/amperka/ino.git
make install 


For Raspberry PI:

apt-get install arduino


This project is bundled with a helpful script that you can place in /usr/local/src.  
The script will download the latest code from the repo and move the files and libraries
to the correct places and build the INO and upload it to the Arduino.  Of course you must have
the command-line INO tool installed that was mentioned above.


I have created helper libraries for different components prefixed with the word "Simple"

Derived from these classes are classes that can handle messages from the serial port. 


Message format:

Byte  0   = 0-D Hexadecimal value of the Pin (0-13)
Byte  1   = A,D A=analog pin.  D=digital pin.
Bytes 2,3 = First command (0-255 bits represented in HEX 0-F)
Bytes 4,5 = Second command 

Example:

AD05B4 - On digital pin 10, the first command is a 5 and the second command is a 180.

The code will know that pin 10 is a servo and tell it to move 180 degrees at speed 5.