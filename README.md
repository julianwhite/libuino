libuino
=======

PC-Arduino Serial Communication Library

copyright (c) 2013 Julian White


LGPL LICENSE
============
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 3 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details at
http://www.gnu.org/licenses/lgpl.html


GENERAL INFORMATION
===================
This project consists of two libraries: (1) a library to use in
your C/C++ programs to interface with an Arduino through serial
UART/USB and (2) a library to use in your Arduino sketches to
interface with a computer through the serial port.  Include the
libuino.h header in your computer/PC side code.  Include the
arduino_libuino.h header in your Arduino sketches.  The functions
in these libraries are designed to communicate with one another
through the serial port.

These libraries are intended: (1) to ease serial port communications
with an Arduino by creating an easy way to build serial connections
and configure them, (2) to manage multiple connections to multiple
Arduinos at once, (3) to support sending and receiving raw byte data
and numeric data, (4) to abstract away endianness and provide an 
"endian safe" way to transfer numeric data such as sensor data,
and (5) to offer some convenience for converting character data
streams into usable numeric data types such as ints, longs, floats,
and doubles.


COMPATIBILITY
=============
This library is not ANSI C compliant as serial communications are
not ANSI C compliant, but OS/implementation dependent.  For that
reason the library was designed to be POSIX compliant and should 
work on any POSIX system such as Raspbian, Debian, Ubuntu, Fedora,
ArchLinux, BSD, Solaris, MacOS, etc.  DOS/Windows users may need
to use Cygwin to implement POSIX functionality, although this has
not been tested.  If anyone has any stories to tell on getting the
library to work on Windows, I am sure a goodly portion of the 
Arduino community would appreciate that.  I fully intend to offer
better support for the Windows community in the near future, so if
you have machines to test on, I will try to incorporate any feedback
you can provide.  For the time being, as this is an open-source
library, testing has been extended to the open-source community
first, especially the Raspberry Pi community.


INSTALLING LIBUINO AS SHARED LIBRARY (LINUX)
============================================
To install the libuino library as a shared library
and to be able to dynamically link the lib into your
applications using the -luino option, run these commands:

	gcc -c -Wall -Werror -fPIC libuino.c
	gcc -shared -o libuino.so libuino.o
	sudo cp libuino.so /usr/local/lib/
	sudo cp libuino.h /usr/local/include/
	cd /usr/local/lib
	sudo ldconfig

Obviously if you're not logged in as root, you need to sudo
when copying the files to the /usr/local/ subdirectories and
also when running the ldconfig.  Otherwise if you're root,
just leave the "sudo" part off. Alternately you can copy the
file to /usr/lib/ and /usr/include/ instead, but, in that case,
be sure to run the ldconfig command from the /usr/lib/ directory.


INSTALLING ARDUINO_LIBUINO
==========================
To install the arduino-side library, arduino_libuino, into your
Arduino environment, simply copy the arduino_libuino folder into
your arduino libraries folder, e.g.:

	cp -r arduino_libuino /usr/share/arduino/libraries/

From then on, simply include the arduino_libuino.h file in your
Arduino sketches.  The functions in the arudino_libuino library
are mainly for raw data transfers.  If you want to send or receive
character data, you may use the standard Arduino Serial functions
such as Serial.print(), Serial.println(), and Serial.read().
Just keep in mind that ASCII character frames need to be terminated
by a new line character ('\n') to signal the end of the stream.
This is done automatically when you use Serial.println().


COMPILING EXAMPLES (LINUX)
==========================
If you have installed libuino as a shared library simply 
cd into the libuino/examples/ directory and do this:

	gcc libuino_helloworld.c -luino -o hello

The -luino option should dynamically link in the shared library.

Strictly speaking it is not required to install as a shared library.
You can always compile the examples using libuino as a bundled
library. Simply copy the library files, libuino.h and libuino.c
into the examples directory and run this command:

	gcc libuino.h libuino.c libuino_helloworld.c -o hello

Please keep in mind that bundled libraries are widely frowned upon
in the open-source/linux community, and for your real applications,
you will want to install libuino as a shared or static library.
	
On the Arduino side, after installing arduino_libuino into your
Arduino environment, the arduino_libuino/examples should appear
in your examples in your Arduino IDE.  If not then you may just
copy each of the example projects into you sketchbook manually.
Please note that for each and every example C program in the 
libuino/examples/ folder, there is a corresponding, similarly
named sketch (.ino file) in libuino/arduino_libuino/examples/.

From your Arduino IDE or inotools or commandline AVR compiler of
choice, build and upload libuino_helloworld.ino to your Arduino.

To execute the examples, return to the libuino/examples/ directory,
and, assuming your Arduino port is /dev/ttyACM0, you may just type:

	./hello

All of the included examples default to /dev/ttyACM0, but you can
specify the correct port to use as a runtime argument like this:

	./hello /dev/ttyACM3

or this:

	./hello /dev/ttyUSB0
	
or maybe:

	./hello /dev/com1
	
or whatever the mount point is on your system.


API / DOCUMENTATION
===================
For your reference, I have marked up all the code with Javadoc
style comments, and I have run the library through Doxygen to
generate HTML pages outlining the Libuino API.  You may find this
documentation in the libuino/docs/ folder of the libuino git repo.

THANKS FOR TRYING LIBUINO!!!
HAPPY CODING, EVERYBODY!!!! :-)

