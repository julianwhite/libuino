#include "libuino.h"

int main(int argc, char **argv)
{
	char *port = "/dev/ttyACM0";
	ino_connection_t *cnx = NULL;
	uint8_t vals[8] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
	uint8_t vals2[8] = {0,0,0,0,0,0,0,0};
	int j;
	
	if ( argc > 1 ) port = argv[1];
	cnx = ino_connection_raw(port, B9600, INO_DEFAULT_WAIT, 8, INO_NO_TIMEOUT);
	ino_open(cnx);
	ino_send_raw(cnx, vals, 8);
	ino_read_raw(cnx, vals2, 8);
	for(j=0; j<8; j++) printf("%02x", vals2[j], vals2[j]);
	printf("\n");
	ino_connection_destroy(cnx);
}
