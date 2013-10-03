/*
 * header.h
 *
 * Created: 9/18/2013 3:31:12 PM
 *  Author: andre_000
 */ 


#ifndef HEADER_H_
#define HEADER_H_

#define F_CPU 20000000

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h> //SDA is also known as Two Wire Interface, TWI.

void TWI_init_master(void);	//Initializes the I2C as a master device
void TWI_start(void);	//Sends a start condition. Check Sparkfun's explanation on the timing diagram to better understand the protocol: https://learn.sparkfun.com/tutorials/i2c/protocol.
void TWI_write(char data);	//Requests to write to the slave device.
char TWI_read_data(void);		//Requests to read from the slave device.
void TWI_stop(void);

int X_AxisRead(void);
void AccelInit(void);

//Definitions exclusive to the Accel

#define SDA_ACCEL_WRITE 0b00110010

#define SDA_ACCEL_READ 0b00110011

#define X_AXIS_OUT_LOW 0b0101000

#define X_AXIS_OUT_HIGH 0b0101001

#endif /* HEADER_H_ */