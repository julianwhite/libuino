#include "arduino_libuino.h"

void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
	//int32_t a=2147483647, b=-2147483647, c=1;
	uint32_t a=0x12345678, b=0xffffffff, c=0x00000000;
  if (Serial.available() > 0)
  {
		ino_send_uint32(a);
		ino_send_uint32(b);
		ino_send_uint32(c);
    Serial.read();
  }
}

