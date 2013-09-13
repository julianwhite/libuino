#include "arduino_libuino.h"

/**
 * File: libuino_example3.ino
 * Written: 9/09/2013 by Julian White
 *
 * Description:
 * Example of how to send non-similar numerics types in a text frame
 * through the serial port to the attached computer.  Especially useful
 * for sending floating point numbers.  This sketch should be loaded 
 * onto an Arduino which is attached via serial/USB to a computer
 * running libuino_example3.c to catch the values sent.
 */
 
/* test values to send */
int val1 = -1023;
float val2 = 3.14f;
long val3 = 305419896;
double val4 = 3.141593f;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	/* wait until a character is sent from serial */
  if (Serial.available() > 0)
  {
		Serial.print(val1);			/* send values as strings with print command */
		Serial.print(" "); 			/* space-delimited */
		Serial.print(val2);			/* 2 decimal places default */
		Serial.print(" ");
		Serial.print(val3);
		Serial.print(" ");
		Serial.print(val4,6);		/* 6 decimal places for max precision of 7 */
										/* 7 is the max precision of Arduino floats/doubles */
		Serial.println();				/* new line to end frame */
    Serial.read();					/* clear buffer */
  }
}
