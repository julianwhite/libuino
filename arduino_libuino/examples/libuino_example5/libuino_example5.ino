#include "arduino_libuino.h"

/**
 * File: libuino_example5.ino
 * Written: 9/10/2013 by Julian White
 *
 * Description:
 * Example of how to read integers through the serial port from the attached
 * computer.  This sketch should be loaded onto an Arduino which is attached
 * via serial/USB to a computer running libuino_example5.c for full 
 * bi-directional reading and writing.  The Arduino will read two integers
 * and return the sum to the attached computer.
 */

int32_t val1 = 0;
int32_t val2 = 0;
int32_t val3 = 0;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	/* wait until at least 8 bytes are available */
  if (Serial.available() >= 8)
  {
  	/* read two 4-byte integers from serial */
		val1 = ino_read_int32();
		val2 = ino_read_int32();
		/* sum them */
		val3 = val1 + val2;
		/*send the results back thru serial */
		ino_send_int32(val3);
  }
}
