#include "libuino.h"

int main(int argc, char* argv[])
{
	char *ino_port = "/dev/ttyACM0";	/* default arduino port to open */
	int numread = 0;									/* number of integers read */
	int j = 0;												/* loop index */
	uint32_t int_arr[3] = {0, 0, 0};	/* array of integers read */
	ino_connection_t *cnx = NULL;			/* connection object */
  	
	/* check for port override in argument 1 */
	if ( argc > 1 ) ino_port = argv[1];
	
	printf("Opening port...\n");
	
	/* create connection interface for reading raw ints. */
	/* 3 32-bit ints x 4 bytes each = 12 bytes total */
  cnx = ino_connection_raw(ino_port, B9600, INO_DEFAULT_WAIT, 12, INO_NO_TIMEOUT);
  
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
	/* send byte to trigger arduino to send frame of integers back */
	write(cnx->fd, "0", 1);
	/* read response */
	numread = ino_read_uint32s(cnx, int_arr, 3);
	printf("%d integers read.\n",numread);
	
	/* for each integer read */
	for(j=0; j<numread; j++)
		/* print hexadecimal and decimal values */
		printf("%02x : %u\n",int_arr[j],int_arr[j]);
	printf("\n");

	/* close serial port and free memory */
	ino_connection_destroy(cnx);
  return 0;
}

