/**
 * @file				libuino.h
 * @version		0.1
 * @author			Julian White
 * @copyright	August 26, 2013 by Julian White
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details at
 * http://www.gnu.org/licenses/lgpl.html
 *
 * @section DESCRIPTION
 *
 * C library to ease serial communication with Arduino microcontrollers,
 * especially sending and receiving numeric data to/from an Arduino
 * via UART/serial/USB. Also included in this library are functions
 * to convert string data into numeric types. Libuino allows for both
 * processed character data and unprocessed raw numeric data transfers.
 * In order to write C applications that will interface with an arduino,
 * simply include libuino.h in the code you write for the PC side of
 * the serial connection.
 *
 * In order to send and receive text/character data, simply create
 * a new ino_connection_t object using the ino_connection_new()
 * function.  With this character-oriented configuration, you may
 * call ino_read() to read a string.  If the string returning from
 * the Arduino is known to be a space-delimited string of numeric
 * values, then you may use the ino_cast_int(), ino_cast_long(),
 * ino_cast_float(), and ino_cast_double() functions to quickly
 * cast them to numeric types according to their position number.
 * Although if normally reading all numeric values in the order they
 * appear, you may find using the standard C functions strtol(),
 * strtof(), and strtod() slightly faster if that is important.
 * The ino_cast_*() functions are quite convenient though, especially
 * for quickly grabbing a value at a specific position in the string
 * without having to count white-spaces.
 
 * In order to transfer raw, unprocessed bytes, you should use the
 * ino_connection_raw() function to create a ino_connection_t object.
 * The "raw" functions will send and receive 16-bit and 32-bit signed
 * or unsigned integers. All integers are converted to network byte
 * order (i.e., big endian) for sending and then back into host byte
 * order when receiving integer data. To use the raw data functions
 * in the library, more than likely you will also want to include
 * arduino_libuino.h in most sketches that will need to interface
 * with the libuino library through the serial port.
 */
 
#ifndef LIBUINO_H
#define LIBUINO_H

/* LIBRARIES */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

/* CONSTANTS */
#define INO_DEFAULT_WAIT 3500000
#define INO_NO_WAIT 0
#define INO_NO_TIMEOUT 0

/**
 * Data type for converting between signed and unsigned 16-bit integers and byte arrays.
 * @since 0.1
 */
typedef union _ino_int16
{
	int16_t sig; 						/**< signed integer */
	uint16_t unsig;					/**< unsigned integer */
	unsigned char bytes[2];	/**< byte array */
}
ino_int16;

/**
 * Data type for converting between signed and unsigned 32-bit integers and byte arrays.
 * @since 0.1
 */
typedef union _ino_int32
{
	int32_t sig;						/**< signed integer */
	uint32_t unsig;					/**< unsigned integer */
	unsigned char bytes[4];	/**< byte array */
}
ino_int32;

/**
 * Data type for storing an Arduino connection's settings.
 * @since 0.1
 */
typedef struct _ino_connection_s
{
	char *port;					/**< tty(COM) port */
	int baud;						/**< bit rate */
	int connect_wait;		/**< length of time in microseconds to wait for arduino to reboot
											   on connection. set to zero if arduino model does not reboot
											   automatically on each connection or if autoreboot is disabled. */
	int raw;						/**< whether or not this is a raw connection type.
											   0 = processed/ASCII, terminated by new line character.
											   1 = raw, unprocessed, read an exact number of bytes. */
	int raw_size;				/**< number of bytes expected in each frame. zero and ignored
											   unless raw is set to 1. */
	int raw_timeout;		/**< length of time to wait for arduino between bytes.
											   raw_timeout should be the number of 1/10ths of a second,
											   e.g., raw_timeout=5 would be 5/10 sec or 1/2 sec.
											   0 = never timeout, wait until enough bytes are read.
											   ignored unless raw is set to 1 (TRUE) */
	int fd;							/**< open port; -1 if closed or on error */
}
ino_connection_t;

/**
 * Create a new character-oriented Arduino connection configuration object.
 * Use this function to configure a connection for transfering character data
 * to and from the Arduino through the serial port.  You can receive as many
 * characters as your buffer will hold, but each frame of character data should
 * be terminated by a new line (line feed) character.  This connection can be
 * opened and closed as many times as you like, but must be freed from memory
 * when it is no longer needed by calling ino_connection_destroy(). If your
 * Arduino model does not automatically reboot each time a serial connection is
 * made to it or this feature has been disabled for your Arduino, then you may
 * use INO_NO_WAIT for your connect_wait for faster connection times.  You may
 * also use INO_DEFAULT_WAIT which should be a safe number of microseconds to
 * wait for the Arduino to reboot before trying to configure the connection.
 * The baud constants are part of the C standard library and are included in
 * the termios.h or bits/termios.h library header, e.g., B9600 or B115200.
 * @param port String representation of port to use, e.g., ttyACM0 or COM1.
 * @param baud Bit-rate, i.e., one of B4800, B9600, B19200,... B115200, etc.
 * @param connect_wait Microseconds to wait for Arduino to auto-reboot on connect.
 * @return Pointer to new ino_connection_t object containing new configuration.
 * @since 0.1
 */
ino_connection_t *ino_connection_new(char *port, int baud, int connect_wait);

/**
 * Create a new raw, unprocessed Arduino connection configuration object.
 * Use this function to configure a connection for transfering raw byte or integer
 * data to and from the Arduino through the serial port.  This connection can be
 * opened and closed as many times as you like, but must be freed from memory
 * when it is no longer needed by calling ino_connection_destroy(). If your
 * Arduino model does not automatically reboot each time a serial connection is
 * made to it or this feature has been disabled for your Arduino, then you may
 * use INO_NO_WAIT for your connect_wait for faster connection times.  You may
 * also use INO_DEFAULT_WAIT which should be a safe number of microseconds to
 * wait for the Arduino to reboot before trying to configure the connection.
 * The baud constants are part of the C standard library and are included in
 * the termios.h or bits/termios.h library header, e.g., B9600 or B115200.
 * read_timeout is the time to wait between bytes before timing out.  You may
 * use INO_NO_TIMEOUT if you are waiting for an exact number of bytes. Otherwise 
 * the read_timeout should be set to the number of 1/10ths of a second to wait,
 * e.g., a read_timeout of 5 will wait 5/10ths or 1/2 of a second between bytes
 * before giving up altogether.
 * @param port String representation of port to use, e.g., ttyACM0 or COM1.
 * @param baud Bit-rate, i.e., one of B4800, B9600, B19200,... B115200, etc.
 * @param connect_wait Microseconds to wait for Arduino to auto-reboot on connect.
 * @param frame_size_bytes Byte-length of frame(s) to be read.
 * @param read_timeout 1/10ths of a second to wait between bytes before timeout.
 * @return Pointer to new ino_connection_t object containing new configuration.
 * @since 0.1
 */
ino_connection_t *ino_connection_raw(char *port, int baud, int connect_wait, int frame_size_bytes, int read_timeout);

/**
 * Close Arduino connection and free config object from memory. 
 * Use this function when the connection is no longer needed.  To merely
 * close the connection and retain it to be opened again later, use the
 * ino_close() function instead.
 * @param cnx Pointer to ino_connection_t object to be destroyed.
 * @since 0.1
 */
void ino_connection_destroy(ino_connection_t *cnx);

/**
 * Open a configured serial port to an Arduino.
 * Use this function to open a connection created using either of
 * ino_connection_new() or ino_connection_raw().
 * @param cnx Pointer to ino_connection_t object to open.
 * @return -1 on error and zero on success.
 * @since 0.1
 */
int ino_open(ino_connection_t *cnx);

/**
 * Close Arduino connection, but retain the ino_connection_t configuration. 
 * Use this function to close the Arduino connection port temporarily.
 * To completely destroy the ino_connection_t object, freeing memory, use
 * the ino_connection_destroy() function instead.
 * @param cnx Pointer to ino_connection_t object to close.
 * @return -1 on error and zero on success.
 * @since 0.1
 */
int ino_close(ino_connection_t *cnx);

/**
 * Test whether the given ino_connection_t is open.
 * Use this function to determine whether the port is open for reading and
 * writing or if there was an error opening the connection or if the
 * connection has been closed.
 * @param cnx Pointer to ino_connection_t object to test.
 * @return 1 if open and 0 if closed.
 * @since 0.1
 */
int ino_is_open(ino_connection_t *cnx);

/**
 * Read a string from the Arduino.
 * Read processed, canonical, string-oriented i/o, terminated by new line character.
 * The connection must be created using the ino_connection_new() function and
 * opened using ino_open().
 * @param cnx Pointer to ino_connection_t object from which to be read.
 * @param buf String buffer into which to read the string from the Arduino.
 * @param max_bytes Maximum number of bytes to read, e.g., size of buffer.
 * @return -1 on error or number of bytes read otherwise.
 * @since 0.1
 */
int ino_read(ino_connection_t *cnx, char *buf, int max_bytes);

/**
 * Cast white-space delimited string data to int by position in the string.
 * Extract numeric value at position, "pos," in the string.  E.g., in order
 * to extract an int at the beginning of the string, use pos = 0. In order
 * to extract the second numeric value in the string, use pos = 1, etc.
 * @param frame Frame of characters returned by ino_read().
 * @param pos Position in the frame of numeric values, starting with zero.
 * @return The integer read at the given position, or zero on error.
 * @since 0.1
 */
int ino_cast_int(char *frame, int pos);

/**
 * Cast white-space delimited string data to long by position in the string.
 * Extract numeric value at position, "pos," in the string.  E.g., in order
 * to extract a long at the beginning of the string, use pos = 0. In order
 * to extract the second numeric value in the string, use pos = 1, etc.
 * @param frame Frame of characters returned by ino_read().
 * @param pos Position in the frame of numeric values, starting with zero.
 * @return The long read at the given position, or zero on error.
 * @since 0.1
 */
long ino_cast_long(char *frame, int pos);

/**
 * Cast white-space delimited string data to float by position in the string.
 * Extract numeric value at position, "pos," in the string.  E.g., in order
 * to extract a float at the beginning of the string, use pos = 0. In order
 * to extract the second numeric value in the string, use pos = 1, etc.
 * @param frame Frame of characters returned by ino_read().
 * @param pos Position in the frame of numeric values, starting with zero.
 * @return The float read at the given position, or zero on error.
 * @since 0.1
 */
float ino_cast_float(char *frame, int pos);

/**
 * Cast white-space delimited string data to double by position in the string.
 * Extract numeric value at position, "pos," in the string.  E.g., in order
 * to extract a double at the beginning of the string, use pos = 0. In order
 * to extract the second numeric value in the string, use pos = 1, etc.
 * @param frame Frame of characters returned by ino_read().
 * @param pos Position in the frame of numeric values, starting with zero.
 * @return The double read at the given position, or zero on error.
 * @since 0.1
 */
double ino_cast_double(char *frame, int pos);

/* read raw, non-canonical byte i/o, exact number of bytes known to each end */
/**
 * Read raw bytes from a raw Arduino serial connection.
 * Reads an exact number of raw bytes from the serial port into an array of
 * uint8_t.  Use this function for more complex or custom interpretation
 * of the bytes delivered.  The function requires an ino_connection_t object
 * created using ino_connection_raw().
 * @param cnx Pointer to ino_connection_t object from which to read data.
 * @param buf Pointer to the beginning of an array of uint8_t.
 * @param frame_size_bytes The exact number of bytes to read.
 * @return The number of bytes read, or -1 on error.
 * @since 0.1
 */
int ino_read_raw(ino_connection_t *cnx, uint8_t *buf, int frame_size_bytes);

/**
 * Read a single signed 16-bit integer from the given Arduino raw connection.
 * Reads 2 bytes from the serial port and converts them from network byte order
 * into host byte order and casts them to a signed 16-bit integer.  The 
 * corresponding Arduino library "send" function will send the integer byte by
 * byte in network byte order.  The function requires an ino_connection_t object
 * created using ino_connection_raw() with a 2 byte frame size.
 * @param cnx Pointer to ino_connection_t object from which to read data.
 * @return The int16_t read from the serial port.
 * @since 0.1
 */
int16_t ino_read_int16(ino_connection_t *cnx);

/**
 * Read a single unsigned 16-bit integer from the given Arduino raw connection.
 * Reads 2 bytes from the serial port and converts them from network byte order
 * into host byte order and casts them to an unsigned 16-bit integer.  The 
 * corresponding Arduino library "send" function will send the integer byte by
 * byte in network byte order.  The function requires an ino_connection_t object
 * created using ino_connection_raw() with a 2 byte frame size.
 * @param cnx Pointer to ino_connection_t object from which to read data.
 * @return The uint16_t read from the serial port.
 * @since 0.1
 */
uint16_t ino_read_uint16(ino_connection_t *cnx);

/**
 * Read multiple signed 16-bit integers from an Arduino raw connection at once.
 * Reads 2 bytes per integer from the serial port and converts them from network
 * byte order into host byte order and casts them to signed 16-bit integers. 
 * The corresponding Arduino library "send" function will send the integer byte by
 * byte in network byte order.  The function requires an ino_connection_t object
 * created using ino_connection_raw() with a 2 bytes per integer frame size,
 * i.e., three 2-byte integers would require a connection frame size of 6 bytes.
 * @param cnx Pointer to ino_connection_t object from which to read data.
 * @param int_buf Pointer to the beginning of an array of int16_t.
 * @param int_count The number of integers to attempt reading <= array length.
 * @return The number of integers read from the serial port.
 * @since 0.1
 */
int ino_read_int16s(ino_connection_t *cnx, int16_t *int_buf, int int_count);

/**
 * Read multiple unsigned 16-bit integers from an Arduino raw connection at once.
 * Reads 2 bytes per integer from the serial port and converts them from network
 * byte order into host byte order and casts them to unsigned 16-bit integers. 
 * The corresponding Arduino library "send" function will send the integer byte by
 * byte in network byte order.  The function requires an ino_connection_t object
 * created using ino_connection_raw() with a 2 bytes per integer frame size,
 * i.e., three 2-byte integers would require a connection frame size of 6 bytes.
 * @param cnx Pointer to ino_connection_t object from which to read data.
 * @param int_buf Pointer to the beginning of an array of uint16_t.
 * @param int_count The number of integers to attempt reading <= array length.
 * @return The number of integers read from the serial port.
 * @since 0.1
 */
int ino_read_uint16s(ino_connection_t *cnx, uint16_t *int_buf, int int_count);

/**
 * Read a single signed 32-bit integer from the given Arduino raw connection.
 * Reads 4 bytes from the serial port and converts them from network byte order
 * into host byte order and casts them to a signed 32-bit integer.  The 
 * corresponding Arduino library "send" function will send the integer byte by
 * byte in network byte order.  The function requires an ino_connection_t object
 * created using ino_connection_raw() with a 4 byte frame size.
 * @param cnx Pointer to ino_connection_t object from which to read data.
 * @return The int32_t read from the serial port.
 * @since 0.1
 */
int32_t ino_read_int32(ino_connection_t *cnx);

/**
 * Read a single unsigned 32-bit integer from the given Arduino raw connection.
 * Reads 4 bytes from the serial port and converts them from network byte order
 * into host byte order and casts them to an unsigned 32-bit integer.  The 
 * corresponding Arduino library "send" function will send the integer byte by
 * byte in network byte order.  The function requires an ino_connection_t object
 * created using ino_connection_raw() with a 4 byte frame size.
 * @param cnx Pointer to ino_connection_t object from which to read data.
 * @return The uint32_t read from the serial port.
 * @since 0.1
 */
uint32_t ino_read_uint32(ino_connection_t *cnx);

/**
 * Read multiple signed 32-bit integers from an Arduino raw connection at once.
 * Reads 4 bytes per integer from the serial port and converts them from network
 * byte order into host byte order and casts them to signed 32-bit integers. 
 * The corresponding Arduino library "send" function will send the integer byte by
 * byte in network byte order.  The function requires an ino_connection_t object
 * created using ino_connection_raw() with a 4 bytes per integer frame size,
 * i.e., three 4-byte integers would require a connection frame size of 12 bytes.
 * @param cnx Pointer to ino_connection_t object from which to read data.
 * @param int_buf Pointer to the beginning of an array of int32_t.
 * @param int_count The number of integers to attempt reading <= array length.
 * @return The number of integers read from the serial port.
 * @since 0.1
 */
int ino_read_int32s(ino_connection_t *cnx, int32_t *int_buf, int int_count);

/**
 * Read multiple unsigned 32-bit integers from an Arduino raw connection at once.
 * Reads 4 bytes per integer from the serial port and converts them from network
 * byte order into host byte order and casts them to unsigned 32-bit integers. 
 * The corresponding Arduino library "send" function will send the integer byte by
 * byte in network byte order.  The function requires an ino_connection_t object
 * created using ino_connection_raw() with a 4 bytes per integer frame size,
 * i.e., three 4-byte integers would require a connection frame size of 12 bytes.
 * @param cnx Pointer to ino_connection_t object from which to read data.
 * @param int_buf Pointer to the beginning of an array of uint32_t.
 * @param int_count The number of integers to attempt reading <= array length.
 * @return The number of integers read from the serial port.
 * @since 0.1
 */
int ino_read_uint32s(ino_connection_t *cnx, uint32_t *int_buf, int int_count);

/**
 * Send an array of raw bytes to the Arduino through the serial port.
 * Sends an array of uint8_t through the given ino_connection_t/serial port.
 * @param cnx Pointer to ino_connection_t object through which to send data.
 * @param buf Array of uint8_t bytes to send to the Arduino.
 * @param frame_size_bytes The number of bytes to be sent to the Arduino.
 * @return The number of bytes actually sent, or -1 on error.
 * @since 0.1
 */
int ino_send_raw(ino_connection_t *cnx, uint8_t *buf, int frame_size_bytes);

/**
 * Send a single signed 16-bit integer to the Arduino through the serial port.
 * Converts an int16_t to network byte order and sends it byte by byte through
 * the given serial port connection to the Arduino on the other end.  Network
 * byte order raw integers can be read by the Arduino using the corresponding
 * "read" function in the Arduino library, arduino_libuino.h .
 * @param cnx Pointer to ino_connection_t object through which to send data.
 * @param val int16_t to be sent to Arduino.
 * @return The number of bytes actually sent, or -1 on error.
 * @since 0.1
 */
int ino_send_int16(ino_connection_t *cnx, int16_t val);

/**
 * Send a single unsigned 16-bit integer to the Arduino through the serial port.
 * Converts an uint16_t to network byte order and sends it byte by byte through
 * the given serial port connection to the Arduino on the other end.  Network
 * byte order raw integers can be read by the Arduino using the corresponding
 * "read" function in the Arduino library, arduino_libuino.h .
 * @param cnx Pointer to ino_connection_t object through which to send data.
 * @param val uint16_t to be sent to Arduino.
 * @return The number of bytes actually sent, or -1 on error.
 * @since 0.1
 */
int ino_send_uint16(ino_connection_t *cnx, uint16_t val);

/**
 * Send a single signed 32-bit integer to the Arduino through the serial port.
 * Converts an int32_t to network byte order and sends it byte by byte through
 * the given serial port connection to the Arduino on the other end.  Network
 * byte order raw integers can be read by the Arduino using the corresponding
 * "read" function in the Arduino library, arduino_libuino.h .
 * @param cnx Pointer to ino_connection_t object through which to send data.
 * @param val int32_t to be sent to Arduino.
 * @return The number of bytes actually sent, or -1 on error.
 * @since 0.1
 */
int ino_send_int32(ino_connection_t *cnx, int32_t val);

/**
 * Send a single unsigned 32-bit integer to the Arduino through the serial port.
 * Converts a uint32_t to network byte order and sends it byte by byte through
 * the given serial port connection to the Arduino on the other end.  Network
 * byte order raw integers can be read by the Arduino using the corresponding
 * "read" function in the Arduino library, arduino_libuino.h .
 * @param cnx Pointer to ino_connection_t object through which to send data.
 * @param val uint32_t to be sent to Arduino.
 * @return The number of bytes actually sent, or -1 on error.
 * @since 0.1
 */
int ino_send_uint32(ino_connection_t *cnx, uint32_t val);

/**
 * Send a single char through the serial port.
 * Sends a single character through the serial port to the Arduino.  This is just
 * a wrapper function for
 * @code
 * 	write(cnx->fd, c, 1);
 * @endcode
 * @param c The character to send to the Arduino.
 * @return 1 on success, or -1 on failure.
 * @since 0.1
 */
 int ino_send_char(ino_connection_t *cnx, char c);

#endif
