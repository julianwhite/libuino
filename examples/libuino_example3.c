#include "libuino.h"

#define BUFFER_SIZE 256

int main(int argc, char* argv[])
{
	char *ino_port = "/dev/ttyACM0";	/* default arduino port to open */
	ino_connection_t *cnx = NULL;			/* connection object */
	char buffer[BUFFER_SIZE];					/* string buffer for incoming frame */
	int numread = 0;									/* number of bytes read */
	int val1 = 0;											/* 1st value read from string */
	float val2 = 0.0f;								/* 2nd value */
	long val3 = 0;										/* 3rd value */
	double val4 = 0.0f;								/* 4th value */
  	
	/* check for port override in argument 1 */
	if ( argc > 1 ) ino_port = argv[1];
	
	printf("Opening port...\n");
	
	/* create connection interface for reading string data */
  cnx = ino_connection_new(ino_port, B9600, INO_DEFAULT_WAIT);
  
  /* open the serial connection to the arduino */
  ino_open(cnx);
  
  /* test connection */
  if ( !ino_is_open(cnx) )
  {
  	printf("ERROR: Unable to connect to Arduino\n");
  	return 0;
  }

	/* try communicating with the arduino */
	printf("Listening...\n");
	/* clear buffer */
	memset(buffer,'\0',BUFFER_SIZE);
	/* send byte to trigger arduino to send frame of characters back */
	write(cnx->fd, "0", 1);
	/* read response */
	numread = ino_read(cnx, buffer, BUFFER_SIZE);
	printf("String returned: %s\n",buffer);
	
	/* extract numeric values from the returned string */
	val1 = ino_cast_int(buffer,0); /* first val at position zero */
	val2 = ino_cast_float(buffer,1);
	val3 = ino_cast_long(buffer,2);
	val4 = ino_cast_double(buffer,3);
	
	/* print numeric values for each value found in returned string */
	printf("As numeric datatypes:\n");
	printf("Value 1: %d\n", val1);
	printf("Value 2: %4.2f\n", val2);
	printf("Value 3: %ld\n", val3);
	printf("Value 4: %4.6lf\n", val4);

	/* close serial port and free memory */
	ino_connection_destroy(cnx);
  return 0;
}

