#include "arduino_libuino.h"

uint8_t vals[8] = {0,0,0,0,0,0,0,0};

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	if ( Serial.available() >= 8 )
	{
		ino_read_raw(vals, 8);
		ino_send_raw(vals, 8);
	}
}
