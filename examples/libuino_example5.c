#include "libuino.h"

/**
 * File: libuino_example5.c
 * Written: 9/10/2013 by Julian White
 *
 * Description:
 * Example of how to send integers through the serial port to an
 * attached Arduino.  This program should be compiled and executed
 * on a computer which is attached to an Arduino running the sketch,
 * libuino_example5.ino, for full bi-directional reading and writing.
 * This program will send two integers to the Arduino to be summed.
 */

int main(int argc, char* argv[])
{
	char *ino_port = "/dev/ttyACM0";	/* default arduino port to open */
	ino_connection_t *cnx = NULL;			/* connection object */
	int numread = 0;									/* number of bytes read */
	int32_t val1 = 0xFF00FF00;
	int32_t val2 = 0x00FF00FF;
	int32_t val3 = 0x00000000;
  	
	/* check for port override in argument 1 */
	if ( argc > 1 ) ino_port = argv[1];
	
	printf("Opening port...\n");
	
	/* configure new serial connection for reading raw integer data */
  cnx = ino_connection_raw(ino_port, B9600, INO_DEFAULT_WAIT, 4, INO_NO_TIMEOUT);
  
  /* open the serial connection to the arduino */
  ino_open(cnx);
  
  /* test connection */
  if ( !ino_is_open(cnx) )
  {
  	printf("ERROR: Unable to connect to Arduino\n");
  	return 0;
  }

	/* send two numbers to be summed to the arduino */
	ino_send_int32(cnx,val1);
	ino_send_int32(cnx,val2);

	/* read sum of two numbers from arduino */
	val3 = ino_read_int32(cnx);
	
	/* output results */
	printf("Sum of two signed 32-bit integers sent: \n");
	printf("0x%08x : %10d\n",val1,val1);
	printf("0x%08x : %10d\n",val2,val2);
	printf("-----------------------\n");
	printf("0x%08x : %10d\n",val3,val3);
	printf("returned from Arduino.\n");

	/* close serial port and free memory */
	ino_connection_destroy(cnx);
  return 0;
}

