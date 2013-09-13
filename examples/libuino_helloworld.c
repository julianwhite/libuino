/**
 * File: libuino_helloworld.c
 * Written: 9/12/2013 by Julian White
 *
 * Description:
 * "Hello World" example showing simplest implementation of the libuino library.
 */

#include "libuino.h"

#define BUFFER_SIZE 64

int main(int argc, char **argv)
{
	char *port = "/dev/ttyACM0";	/* default serial port */
	int baud = B9600;							/* bit rate */
	ino_connection_t *cnx = NULL;	/* arduino connection settings */
	char buffer[BUFFER_SIZE];			/* message buffer */
	
	/* if a serial port was passed in command line arguments,
	   use that one instead of the default /dev/ttyACM0 */
	if ( argc > 1 ) port = argv[1];
	
	/* create a 9600 baud serial connection to arduino */
	cnx = ino_connection_new(port, baud, INO_DEFAULT_WAIT);
	
	/* open the new connection interface */
	ino_open(cnx);
	
	/* if port opened successfully */
	if ( ino_is_open(cnx) )
	{
		/* send byte to trigger arduino to send message */
		ino_send_char(cnx, '0');
		
		/* read the arduino's response into the buffer */
		ino_read(cnx, buffer, BUFFER_SIZE);
		
		/* print results */
		printf("%s\n", buffer);
	}
	
	/* close serial port and free memory */
	ino_connection_destroy(cnx);
	
	return 0;
}
