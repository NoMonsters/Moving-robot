/*
 * Normal_PI_Reg.cpp
 *
 * Created: 25.05.2018 22:16:25
 * Author : Vasiliy
 */ 

#define F_CPU 16000000UL

#include <stdlib.h>
#include <stdio.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
#include <string.h>


#include "Lib/UART/UART.h"
#include "Lib/i2C/I2C_Master.h"
#include "Lib/IMU6050/IMU6050.h"	
#include "Lib/GPSparse/parseGPS.h"
#include "Lib/NAV/Navigation.h"
#include "Lib/Hardware/HardwareSetup.h"
#include "Lib/Hardware/Time.h"
#include "Lib/PI//Regulators.h"
#include "Lib/Encoder/GetSpeed.h"


//*******************глобальные переменные*******************
volatile uint8_t Left_Encoder_counter = 0;	//количество срабатываний энкодера. Левое колесо
volatile uint8_t Right_Encoder_counter = 0;	//количество срабатываний энкодера. Правое колесо
volatile uint32_t OVF_counter = 0;			//количество переполнений таймера TIM2

volatile uint8_t Call_PI_reg = 0;
volatile uint8_t Call_GPS = 0;
volatile uint8_t Calc_Omega = 0;           //флаг по которому вызывается расчет угловой скорости на цель
volatile uint8_t Transmit_UART = 0;

const uint8_t How_often_Transmit_UART = 60;
const uint8_t How_often_call_PI_reg = 20;
const uint8_t How_often_call_GPS = 40;// начиная с периода вызова 0.17 (с) вывод некорректный
const uint8_t How_often_calc_omega = 40;

//***********************************************************

double limiter(double MIN, double MAX, double value)
{
	if(value > MAX)
	value = MAX;
	if(value < MIN)
	value = MIN;
	return value;
}

void Init_all()
{
	PWN_for_wheels_init();						//инициализация ШИМ для колес
	Encoder_interrupts_init();					//инициализация прерываний для энкодера
	Timer_for_wheels_init();					//инициализация таймера для колес
	Motor_diraction_init();
	UART_init();
	I2C_Init();
	MPU6050_Init();

}

ISR(INT0_vect)
{
	Left_Encoder_counter++;
}
ISR(INT1_vect)
{
	Right_Encoder_counter++;
}
ISR(TIMER2_OVF_vect)
{
	OVF_counter++;
	Call_PI_reg++;		//вызывается каждые 10 переполнения таймера
	Call_GPS++;
	Transmit_UART++;
	Calc_Omega++;
	
}

int main(void)
{
	char GPSstatus[1], GPS_NS[1], GPS_WE[1];
	char GPS_str[80];
	char GPS_symbol_buff;
	uint8_t GPS_str_symbol_index = 0;
	bool GPS_str_is_ready = 0;
	
	double GPSlatitude = 0, GPSlongitude = 0, GPS_spd = 0, GPS_hdg = 0;
	double omegaAim = 0;
	double latitudeAim = 55.7667;
	double longitudeAim = 37.6804;
	
	uint32_t Last_speed_check_right = 0, Last_speed_check_left = 0;
	float Current_speed_left = 0;
	float Current_speed_right = 0;
	
	float Raw_gyro_X_Y_Z_values[] = {0, 0, 0}, Real_gyro_X_Y_Z_values[] = {0, 0, 0};
	float Omega_LR_required[] = {0, 0};
		
	float Velocity = 0.4;
	
	bool straight = 0;
	char Float_to_char_buffer[10];
	
	
	Init_all();
	sei();//разрешаем глобальные прерывания
	
    while (1) 
    {
		
		
		
		//*************************Чтение данных с гироскопа**************************
		Read_RawValue(Raw_gyro_X_Y_Z_values);
		for (int i=0; i<=2; i++)
		{
			Real_gyro_X_Y_Z_values[i] = Raw_gyro_X_Y_Z_values[i]/939.6544;//пересчет сырых данных в реальные Raw_gyro_X_Y_Z_values[i]/16.4/57.296, где 57.296 - пересчет в рад/с
		}
			
		
		
		
		//*************************Преобразование строки с GPS********************************
		if(GPS_str_is_ready == true)
		{
			parseGPS(GPS_str, GPSstatus, GPSlatitude, GPS_NS, GPSlongitude, GPS_WE, GPS_spd, GPS_hdg);
			GPS_str_is_ready = false;
		}
		
		
		
		//**************************Вычисление угловой скорости на цель**************************
		if(GPS_hdg != 0.0)
		{
			if(Calc_Omega >= How_often_calc_omega)
			{
				omegaAim = 0.1 * getOmegaAim(longitudeAim, latitudeAim, GPSlongitude, GPSlatitude, GPS_hdg, GPS_spd);
				Calc_Omega = 0;
				//dtostrf(omegaAim, 4, 5, Float_to_char_buffer);
				//UART_send_Str(Float_to_char_buffer);
				//UART_send_char('\n');
			}
		}
		else 
		{
			omegaAim = 0.0;
		}
		
			
		
		
		
		
		//*************************Считывание скоростей колес*******************************
		
		//считываем значение скорости ЛЕВОГО колеса каждые How_many_speed_slots щелей
		if(Left_Encoder_counter >= How_many_speed_slots)
		{		
			Get_speed_left(Current_speed_left);
			Last_speed_check_left = Get_time();
		}
		
		//считываем значение скорости ПРАВОГО колеса каждые How_many_speed_slots щелей
		if(Right_Encoder_counter >= How_many_speed_slots)
		{
			Get_speed_right(Current_speed_right);
			Last_speed_check_right = Get_time();
		}
		
		Does_speed_zero(Last_speed_check_left, Last_speed_check_right, Current_speed_left, Current_speed_right);

		
		
		
		
		//*************************Применение регулирования*******************************
		if(Call_PI_reg >= How_often_call_PI_reg)
		{
			Turn_control(omegaAim, Velocity, Real_gyro_X_Y_Z_values[2], Omega_LR_required, straight);
			OCR0B = static_cast<uint8_t>(limiter(Min_output_for_Reg, Max_output_for_Reg,Apply_regulator_right(Current_speed_right, Omega_LR_required[1], 0.7, 0.02)));//Omega_LR_required[1], 0.51, 0.15))); //0.187, 0.0712 0.208, 0.098
			OCR0A = static_cast<uint8_t>(limiter(Min_output_for_Reg, Max_output_for_Reg,Apply_regulator_left(Current_speed_left, Omega_LR_required[0], 0.7, 0.02)));//Omega_LR_required[0], 0.51, 0.09)));  //0.207, 0.0792 0.208, 0.098
					
			Call_PI_reg = 0;
		}
		
		
		
		
		//*************************Прием GPS и формирование строки*******************************
		if(Call_GPS >= How_often_call_GPS)
		{
			GPS_symbol_buff = UART_get_char();
			if (GPS_symbol_buff == '$')
			{
				GPS_str[GPS_str_symbol_index] = GPS_symbol_buff;
				while(GPS_symbol_buff != '\n')
				{
					GPS_symbol_buff = UART_get_char();
					GPS_str_symbol_index++;
					GPS_str[GPS_str_symbol_index] = GPS_symbol_buff;
				}	
				GPS_str[GPS_str_symbol_index+1] = '\0';
				GPS_str_symbol_index = 0;
				GPS_str_is_ready = true;
			}	
			
			Call_GPS = 0;
		}
		
		
		
		
		//*************************Передача в порт для тестов*******************************
		if(Transmit_UART > How_often_Transmit_UART)
		{
			dtostrf(Real_gyro_X_Y_Z_values[2], 4, 5, Float_to_char_buffer);
			UART_send_Str(Float_to_char_buffer);
			UART_send_char('\n');
			dtostrf(omegaAim, 4, 5, Float_to_char_buffer);
			UART_send_Str(Float_to_char_buffer);
			UART_send_char('\n');
			UART_send_Str(GPS_str);
			UART_send_char('\n');
			UART_send_char('\n');
			Transmit_UART = 0;
		}
		
		
		
		/*if(GPS_str_is_ready == true)
		{
			UART_send_Str(GPS_str);
			UART_send_char('\n');
			//dtostrf(GPSlongitude, 4, 5, Float_to_char_buffer);
			//UART_send_Str(Float_to_char_buffer);
			//UART_send_char('\n');
			//dtostrf(omegaAim*57.296, 4, 5, Float_to_char_buffer);
			dtostrf(Real_gyro_X_Y_Z_values[2], 4, 5, Float_to_char_buffer);
			UART_send_Str(Float_to_char_buffer);
			//UART_send_char(' ');
			//dtostrf(GPS_hdg, 4, 5, Float_to_char_buffer);
			//UART_send_Str(Float_to_char_buffer);
			UART_send_char('\n');
			UART_send_char('\n');
		}*/
    }
}

