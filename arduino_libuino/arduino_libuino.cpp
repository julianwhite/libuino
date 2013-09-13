/**
 * @file				arduino_libuino.cpp
 * @version		0.1
 * @author			Julian White
 * @copyright	August 28, 2013 Julian White
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
 * Arduino library to ease serial communication with computers,
 * particularly sending and receiving numeric data to/from
 * a computer via UART/serial/USB. This library will send and
 * receive 16-bit and 32-bit signed or unsigned integers.
 * All integers are converted to network byte order for sending
 * and back into host byte order when receiving integer data.
 * To use this library, more than likely you will also want to 
 * use arduino_libuino's sister library, a.k.a simply "libuino,"
 * on the attached computer on the other side of the Arduino
 * serial port.  To write C applications that will interface
 * with this arduino_libuino library, simply include libuino.h
 * in the code you write for the PC side of the serial connection.
 */

#include "arduino_libuino.h"

/** 
 * Read 16-bit signed integer from computer through serial port.
 * Function to read a raw signed 16-bit integer from the Serial
 * interface in network byte order (i.e., big endian). Serial
 * interface must be initialized with Serial.begin() prior to
 * function call. More than likely you will be sending this
 * integer from the corresponding libuino.h "send" function
 * from the attached computer.
 * @return 16-bit signed integer read.
 * @since 0.1
 */
int16_t ino_read_int16()
{
	ino_int16 i;
	
	i.unsig = 0;
	i.unsig = Serial.read();
	i.unsig <<= 8;
	i.unsig |= Serial.read();

	return i.sig;
}

/** 
 * Read 16-bit unsigned integer from computer through serial port.
 * Function to read a raw unsigned 16-bit integer from the Serial
 * interface in network byte order (i.e., big endian). Serial
 * interface must be initialized with Serial.begin() prior to
 * function call. More than likely you will be sending this
 * integer from the corresponding libuino.h "send" function
 * from the attached computer.
 * @return 16-bit unsigned integer read.
 * @since 0.1
 */
uint16_t ino_read_uint16()
{
	ino_int16 i;
	
	i.unsig = 0;
	i.unsig = Serial.read();
	i.unsig <<= 8;
	i.unsig |= Serial.read();

	return i.unsig;
}

/** 
 * Read 32-bit signed integer from computer through serial port.
 * Function to read a raw signed 32-bit integer from the Serial
 * interface in network byte order (i.e., big endian). Serial
 * interface must be initialized with Serial.begin() prior to
 * function call. More than likely you will be sending this
 * integer from the corresponding libuino.h "send" function
 * from the attached computer.
 * @return 32-bit signed integer read.
 * @since 0.1
 */
int32_t ino_read_int32()
{
	ino_int32 i;
	int j;
	
	i.unsig = 0;
	for(j=0; j<4; j++)
	{
		i.unsig <<= 8;
		i.unsig |= Serial.read();
	}
	
	return i.sig;
}

/** 
 * Read 32-bit unsigned integer from computer through serial port.
 * Function to read a raw unsigned 32-bit integer from the Serial
 * interface in network byte order (i.e., big endian). Serial
 * interface must be initialized with Serial.begin() prior to
 * function call. More than likely you will be sending this
 * integer from the corresponding libuino.h "send" function
 * from the attached computer.
 * @return 32-bit unsigned integer read.
 * @since 0.1
 */
uint32_t ino_read_uint32()
{
	ino_int32 i;
	int j;
	
	i.unsig = 0;
	for(j=0; j<4; j++)
	{
		i.unsig <<= 8;
		i.unsig |= Serial.read();
	}
	
	return i.unsig;
}

/** 
 * Send 16-bit signed integer to computer through serial port.
 * Function to send a raw signed 16-bit integer across the Serial
 * interface in network byte order (i.e., big endian). Serial
 * interface must be initialized with Serial.begin() prior to
 * function call. More than likely you will be reading this
 * integer with a corresponding libuino.h "read" function
 * on the attached computer.
 * @param	 n 16-bit signed integer to send.
 * @since 0.1
 */
void ino_send_int16(int16_t n)
{
	ino_int16 i;
	i.sig = n;
	Serial.write(highByte(i.unsig));
	Serial.write(lowByte(i.unsig));
}

/** 
 * Send 16-bit unsigned integer to computer through serial port.
 * Function to send a raw unsigned 16-bit integer across the Serial
 * interface in network byte order (i.e., big endian). Serial
 * interface must be initialized with Serial.begin() prior to
 * function call. More than likely you will be reading this
 * integer with a corresponding libuino.h "read" function
 * on the attached computer.
 * @param	 n 16-bit unsigned integer to send.
 * @since 0.1
 */
void ino_send_uint16(uint16_t n)
{
	ino_int16 i;
	i.unsig = n;
	Serial.write(highByte(i.unsig));
	Serial.write(lowByte(i.unsig));
}

/** 
 * Send 32-bit signed integer to computer through serial port.
 * Function to send a raw signed 32-bit integer across the Serial
 * interface in network byte order (i.e., big endian). Serial
 * interface must be initialized with Serial.begin() prior to
 * function call. More than likely you will be reading this
 * integer with a corresponding libuino.h "read" function
 * on the attached computer.
 * @param	 n 32-bit signed integer to send.
 * @since 0.1
 */
void ino_send_int32(int32_t n)
{
	uint16_t tmp; /* one 16-bit word at a time */
	ino_int32 i;	/* signed to unsigned converter */
	
	/* set signed value */
	i.sig = n;

	/* higher 16-bit word */
	tmp = i.unsig >> 16;
	//tmp = n >> 16;
	Serial.write(highByte(tmp));
	Serial.write(lowByte(tmp));
	/* lower 16-bit word */
	tmp = i.unsig & 0xFFFF;
	//tmp = n && 0xFFFF;
	Serial.write(highByte(tmp));
	Serial.write(lowByte(tmp));
}

/** 
 * Send 32-bit unsigned integer to computer through serial port.
 * Function to send a raw unsigned 32-bit integer across the Serial
 * interface in network byte order (i.e., big endian). Serial
 * interface must be initialized with Serial.begin() prior to
 * function call. More than likely you will be reading this
 * integer with a corresponding libuino.h "read" function
 * on the attached computer.
 * @param	 n 32-bit unsigned integer to send.
 * @since 0.1
 */
void ino_send_uint32(uint32_t n)
{
	uint16_t tmp; /* one 16-bit word at a time */
	ino_int32 i;	/* signed to unsigned converter */
	
	/* set signed value */
	i.unsig = n;

	/* higher 16-bit word */
	tmp = i.unsig >> 16;
	Serial.write(highByte(tmp));
	Serial.write(lowByte(tmp));
	/* lower 16-bit word */
	tmp = i.unsig & 0xFFFF;
	Serial.write(highByte(tmp));
	Serial.write(lowByte(tmp));
}

/**
 * Read raw bytes from a serial connection.
 * Reads an exact number of raw bytes from the serial port into an array of
 * uint8_t.  Use this function for more complex or custom interpretation
 * of the bytes delivered.
 * @param buf Pointer to the beginning of an array of uint8_t.
 * @param nbytes The exact number of bytes to read.
 * @since 0.1
 */
void
ino_read_raw(uint8_t *buf, int nbytes)
{
  int j;
	for (j=0; j<nbytes; j++)
		buf[j] = Serial.read();
}

/**
 * Send an array of raw bytes to the computer through the serial port.
 * Sends an array of uint8_t through the serial port.
 * @param buf Array of uint8_t bytes to send.
 * @param nbytes The number of bytes to be sent.
 * @since 0.1
 */
void
ino_send_raw(uint8_t *buf, int nbytes)
{
	int j;
	for (j=0; j<nbytes; j++)
		Serial.write(buf[j]);
}

