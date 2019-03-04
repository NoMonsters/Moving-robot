/*
 * IMU6050.cpp
 *
 * Created: 10.01.2019 20:15:56
 *  Author: mlx-0
 */ 
#include "IMU6050.h"

void MPU6050_Init()
{
	_delay_ms(150);										// Power up time >100ms
	I2C_Start_Wait(0xD0);								// Start with device write address
	I2C_Write(SMPLRT_DIV);								// Write to sample rate register
	I2C_Write(0x07);									// 1KHz sample rate
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);								// Write to power management register
	I2C_Write(0x01);									// X axis gyroscope reference frequency
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);									// Write to Configuration register
	I2C_Write(0x00);									// Fs = 8KHz
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);								// Write to Gyro configuration register
	I2C_Write(0x18);									// Full scale range +/- 2000 degree/C
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);								// Write to interrupt enable register
	I2C_Write(0x01);
	I2C_Stop();
}
void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);								/* I2C start with device write address */
	I2C_Write(GYRO_XOUT_H);								/* Write start location address from where to read */
	I2C_Repeated_Start(0xD1);							/* I2C start with device read address */
}
void Read_RawValue(float *Gyro_X_Y_Z_values)
{
	MPU_Start_Loc();									// Read Gyro values
	//Acc_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	//Acc_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	//Acc_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	//Temperature = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_X_Y_Z_values[0] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_X_Y_Z_values[1] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_X_Y_Z_values[2] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
}