void setup()
{
	/* set baud and open serial port */
	Serial.begin(9600);
}

void loop()
{
	/* wait for a byte from computer */
	if ( Serial.available() > 0 )
	{
		/* send response */
		Serial.println("Hello World!");
		
		/* clear the byte sent from computer */
		Serial.read();
	}
}
