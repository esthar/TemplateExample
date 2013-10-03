/*
 * TemplateExample.c
 *
 * Created: 9/18/2013 3:25:22 PM
 *  Author: andre_000
 */ 

#include "header.h"

int main(void)
{

	unsigned int x=0;
	
	TWI_init_master();
	
	AccelInit();
	
	DDRD = 1<<1;
	
    while(1)
    {
        x = X_AxisRead();
		
		if (x<5000)
		{
			PORTD^=1<<1;
		}
		_delay_ms(100);
    }
}

int X_AxisRead(){
	char xh=0;
	char xl=0;
	unsigned int x=0;
	
	 TWI_start();
	 TWI_write(SDA_ACCEL_WRITE);
	 TWI_write(X_AXIS_OUT_LOW);
	 TWI_start();					//This is an example of a repeated start since we never send a stop condition.
	 TWI_write(SDA_ACCEL_READ);	//Notice how the transmission goes. We gave the accel the address for which we wanted access to but since we want to read it we need to start again and send a read request.
	 xl = TWI_read_data();			//The accel will send us the value of the register so we write it to our 8-bit variable x low.
	 TWI_stop();
	 //Much cleaner with the #defines in place huh?
	 TWI_start();
	 TWI_write(SDA_ACCEL_WRITE);
	 TWI_write(X_AXIS_OUT_HIGH);
	 TWI_start();
	 TWI_write(SDA_ACCEL_READ);
	 xh = TWI_read_data();
	 TWI_stop();
	 
	 x = xh<<8 | xl;
	 
	 return x;
}

void AccelInit(void){
		TWI_start();
		TWI_write(0b00110010);	//Then we need to talk specifically to our accelerometer device. We do that by writing it's 7-bit address on the SDA line, which is 0011001. Now if we want to write to it we add a 0 at the end or a 1 to read, making it an 8-bit number.
		//In this case we want to write to it. Check table 11 on page 19 of the datasheet.
		TWI_write(0b0100000);	//Then we send the address of the register we want to write to. In our case that is 0b0100000, control register #1.
		TWI_write(0b01010111);	//Finally we write what we want to that address, which we discussed above to turn on the device, namely 0b01010111.
		TWI_stop();
}

void TWI_init_master(void) // Function to initialize master
{
	TWBR=0x5c;    // Set bit rate
	TWSR=(0<<TWPS1)|(0<<TWPS0);    // Set prescalar bits
	// SCL freq= F_CPU/(16+2(TWBR).(TWPS)) This gives us a frequency of 20MHz/200=100KHz which is within the capabilities of the Accel.
}

void TWI_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0); // Wait till start condition is transmitted. This is important or else later we could end up trying to transmit again while this transmission hadn't ended... a collision of data would happen.
}

void TWI_write(char data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}


char TWI_read_data(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

void TWI_stop(void)
{
	// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
}