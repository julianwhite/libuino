#include "arduino_libuino.h"

/**
 * File: libuino_example4.ino
 * Written: 9/10/2013 by Julian White
 *
 * Description:
 * Example of how to read integers through the serial port from the attached
 * computer.  This sketch should be loaded onto an Arduino which is attached
 * via serial/USB to a computer running libuino_example4.c for full 
 * bi-directional reading and writing.  The Arduino will read two integers
 * and return the sum to the attached computer.
 */

uint16_t val1 = 0;
uint16_t val2 = 0;
uint16_t val3 = 0;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	/* wait until at least 4 bytes are available */
  if (Serial.available() >= 4)
  {
  	/* read two 2-byte integers */
		val1 = ino_read_uint16();
		val2 = ino_read_uint16();
		/* sum them */
		val3 = val1 + val2;
		/* send results back thru serial */
		ino_send_uint16(val3);
  }
}
