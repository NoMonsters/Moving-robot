#include <avr/io.h>
#include "../i2C/I2C_Master.h"
#include "HMC5883L.h"

void HMC5883L_Init()
{
	_delay_ms(150);
	
	I2C_Start(HMC5883L_writeAddres);
	I2C_Write(0x00);
	I2C_Write(0x70);
	I2C_Stop();
	
	I2C_Start(HMC5883L_writeAddres);
	I2C_Write(0x01);
	I2C_Write(0x20);
	I2C_Stop();
	
	I2C_Start(HMC5883L_writeAddres);
	I2C_Write(0x02);
	I2C_Write(0x00);
	I2C_Stop();
}

void HMC5883L_Read(float *X_Y_Z_values)
{
	_delay_ms(6);
	I2C_Start(HMC5883L_readAddres);
	I2C_Write(0x06);
	
	X_Y_Z_values[0] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	X_Y_Z_values[2] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	X_Y_Z_values[1] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	
	I2C_Start(HMC5883L_writeAddres);
	I2C_Write(0x03);
	_delay_ms(70);
	
	I2C_Stop();
}