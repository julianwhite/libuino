#include "arduino_libuino.h"

void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
	int a=13, b=1024, c=-3333;
  if (Serial.available() > 0)
  {
  	ino_send_int16(a);
  	ino_send_int16(b);
  	ino_send_int16(c);

    Serial.read();
  }
}
