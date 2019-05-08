/*
 * Normal_PI_Reg.cpp
 *
 * Created: 25.05.2018 22:16:25
 * Author : Vasiliy
 */ 

#define F_CPU 16000000UL

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "Lib/UART/UART.h"
#include "Lib/i2C/I2C_Master.h"
#include "Lib/IMU6050/IMU6050.h"	
#include "Lib/GPSparse/parseGPS.h"

//*******************глобальные переменные*******************
volatile uint8_t Left_Encoder_counter = 0;	//количество срабатываний энкодера. Левое колесо
volatile uint8_t Right_Encoder_counter = 0;	//количество срабатываний энкодера. Правое колесо
volatile uint8_t Call_UART = 0;
volatile uint32_t OVF_counter = 0;			//количество переполнений таймера TIM2
volatile uint8_t Call_PI_reg = 0;
volatile uint8_t Call_Get_Speed = 0;		//флаг по которому вызывается считывание скорости
volatile uint8_t Call_GPS = 0;
volatile uint8_t Transmit_UART= 0;

const uint8_t How_often_Transmit_UART = 5;
const uint8_t How_many_speed_slots = 5;
const uint8_t How_often_read_GPS = 5;
const uint8_t How_often_call_PI_reg = 20;
const uint8_t How_often_call_UART = 2;
const double Max_output_for_Reg = 255;
const double Min_output_for_Reg = 0;

const uint16_t TIM2_prescaler = 256;
const uint8_t Time_of_one_tic = 16;
const uint16_t Time_of_one_OVF = 255*Time_of_one_tic;
//***********************************************************
void ADC_Init(void)
{
	ADCSRA |= (1<<ADEN)								// Разрешение использования АЦП
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);				//Делитель 128 = 64 кГц
	ADMUX |= (1<<REFS1)|(1<<REFS0);					//Внутренний Источник ОН 2,56в, вход ADC0
}
uint8_t ADC_convert (uint8_t input)
{
	if (input == 1)
		ADMUX |= (1<<MUX0);
	else
		ADMUX &=~ (1<<MUX0);
	ADCSRA |= (1<<ADSC);							//Начинаем преобразование
	while((ADCSRA & (1<<ADSC)));					//проверим закончилось ли аналого-цифровое преобразование
	return static_cast<uint8_t>(ADC>>2);
}

double limiter(double MIN, double MAX, double value)
{
	if(value > MAX)
	value = MAX;
	if(value < MIN)
	value = MIN;
	return value;
}
uint32_t Get_time()
{
	uint8_t Current_counter_value = TCNT2;
	uint32_t Current_Time = OVF_counter*Time_of_one_OVF+Current_counter_value*static_cast<uint32_t>(Time_of_one_tic);
	
	return Current_Time;
}

/*class PI_regulators
{
	protected:
		double Kp;
		double Ki;
	public:
		PI_regulators(): Kp(0.1), Ki(0.01) //конструктор по умолчанию
		{}
		PI_regulators(double value1, double value2): Kp(value1), Ki(value2)//конструктор по двум кэфам
		{}
		void Set_Kp(double value)
		{Kp = value;}
		void Set_Ki(double value)
		{Ki = value;}
		double Apply_regulator(double current_value, double required_value)// const значит тут, что этот метод не может изменять поля класса
		{
			static double output, error;
			static double output_prev = 0, error_prev = 0;// выход и ошибка с предыдушей итерации

			error = required_value - current_value;
			output = output_prev + Kp*(error-error_prev)+Ki*error;
			
			error_prev = error;
			output_prev = output;

			return output;
		}
};*/
double Apply_regulator_left(double current_value, double required_value, float Kp, float Ki)
{
	static double output = 0, error = 0;
	static double output_prev = 0, error_prev = 0;// выход и ошибка с предыдушей итерации

	if (output_prev >= Max_output_for_Reg || output_prev <= Min_output_for_Reg)
		Ki = 0;
	
	error = required_value - current_value;
	output = output_prev + Kp*(error-error_prev)+Ki*error;
	
	error_prev = error;
	output_prev = output;

	return output;
}
/*double Apply_regulator_left_classic(double current_value, double required_value, float Kp, float Ki)
{
	static double output = 0, error = 0;
	static double integral_prev = 0;

	error = required_value - current_value;
	output = Kp*error  + integral_prev + Ki*error;
	
	integral_prev = Ki*error;

	return output;
}*/
/*double Apply_regulator_right_classic(double current_value, double required_value, float Kp, float Ki)
{
	static double output = 0, error = 0;
	static double integral_prev = 0;

	error = required_value - current_value;
	output = Kp*error + integral_prev + Ki*error;
	
	integral_prev = Ki*error;

	return output;
}*/
double Apply_regulator_right(double current_value, double required_value, float Kp, float Ki)
{
	static double output = 0, error = 0;
	static double output_prev = 0, error_prev = 0;// выход и ошибка с предыдушей итерации

	if (output_prev >= Max_output_for_Reg || output_prev <= Min_output_for_Reg)
	Ki = 0;

	error = required_value - current_value;
	output = output_prev + Kp*(error-error_prev)+Ki*error;
					
	error_prev = error;
	output_prev = output;

	return output;
}

double Calc_Omega_additional_using_gyro(double current_value, double required_value, float Kp, float Ki)
{
	static double output = 0, error = 0;
	static double output_prev = 0, error_prev = 0;// выход и ошибка с предыдушей итерации

	error = required_value - current_value;
	output = output_prev + Kp*(error-error_prev)+Ki*error;
	error_prev = error;
	output_prev = output;

	return output;
}
/*void Calc_OmegaLeftReq_OmegaRightReq_for_turn(float Radius, float &omega_const_1, float &omega_const_2, float OMEGA,bool straight)
{
	float const l = 0.067, Wheel_radius = 0.0675;

	omega_const_1 = (Radius - l)*OMEGA/Wheel_radius;
	omega_const_2 = (Radius + l)*OMEGA/Wheel_radius;
} */
void Turn_control(float Radius, float Velocity, float Current_OmegaZ, float *Omega_LR_required, bool straight, char *UART_buf)
{
	static float const Wheel_radius = 0.03375; //from prev function
	float omega_const_1 = 0, omega_const_2 = 0;
	float OMEGA = Velocity/Radius, omega_additional = 0;
	//char Float_to_char_buffer1[5], Float_to_char_buffer2[5];
	
	//Calc_OmegaLeftReq_OmegaRightReq_for_turn(Radius, omega_const_1, omega_const_2, OMEGA, straight);
	
	omega_const_1 = 2*Velocity/Wheel_radius;
	omega_const_2 = 2*Velocity/Wheel_radius;
	
	omega_additional = Calc_Omega_additional_using_gyro(Current_OmegaZ, OMEGA, 0.5, 0.1);
	
	//dtostrf(omega_additional, 5, 2, Float_to_char_buffer1);
	//dtostrf(Current_OmegaZ, 5, 2, Float_to_char_buffer2);
	//sprintf(UART_buf, "omega_ad = %s, current = %s \n", Float_to_char_buffer1, Float_to_char_buffer2);

	Omega_LR_required[0] = 9.55*(omega_const_1 - omega_additional/2); //-9.55*omega_additional/2;
	Omega_LR_required[1] = 9.55*(omega_const_2 + omega_additional/2); //9.55*omega_additional/2;
}

void Encoder_interrupts_init()
{
	DDRD&=~(1<<DDD2);//настраиваем PD2 (INT0) как вход. Левое колесо
	DDRD&=~(1<<DDD3);//настраиваем PD3 (INT1) как вход. Правое колесо
	EIMSK|=(1<<INT0)|//разрешаем прерывания на ноге INT0
			(1<<INT1);//разрешаем прерывания на ноге INT1
	EICRA|=(1<<ISC01)|//прерывание происходит по спадающему изменению уровня на ноге INT0
			(1<<ISC11);//прерывание происходит по спадающему изменению уровня на ноге INT1
	
	//Можно использовать маски чтобы настроить прерывание на другие ноги см даташит	
}
void Motor_diraction_init()
{
	DDRD|=(1<<DDD7);
	PORTD|=(1<<PORTD7);
	DDRB|=(1<<DDB0);
	PORTB&=~(1<<PORTB0);
	
}
void PWN_for_wheels_init()
{
	DDRD|=(1<<DDD6)|(1<<DDD5);//настраиваем PD6 (OC0A) как выход и настраиваем PD5 (OC0B) как выход
	TCCR0A|=(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);//fast PWM, прямой
	TCCR0B|=(1<<CS02);//предделитель 256
	OCR0A = 0;//начальное значение ШИМ 70. Левое колесо
	OCR0B = 0;//начальное значение ШИМ 70. Правое колесо
	
	
}
void Timer_for_wheels_init()
{
	TCCR2B&=~(1<<CS20);
	TCCR2B|=(1<<CS21)|(1<<CS22);	//предделитель 256
	TIMSK2|=(1<<TOIE2);						//устанавливаем прерывания по переполнению
}

void Get_speed_left(float &Current_speed_left)
{
	static uint32_t Prev_call_time_l = 0;
	uint32_t Time_now = Get_time();
			
	Current_speed_left = 15000000.0/(Time_now - Prev_call_time_l);
	Prev_call_time_l = Time_now;	

	Left_Encoder_counter = 0;
} 
void Get_speed_right(float &Current_speed_right)
{
	static uint32_t Prev_call_time_r = 0;
	uint32_t Time_now = Get_time();
	
	Current_speed_right = 15000000.0/(Time_now - Prev_call_time_r);
	Prev_call_time_r = Time_now;

	Right_Encoder_counter = 0;
}
void Does_speed_zero(uint32_t Last_speed_check_left, uint32_t Last_speed_check_right, float &Current_speed_left , float &Current_speed_right)
{
	uint32_t Current_time = Get_time();
	
	if(Current_time - Last_speed_check_left >= 800000)
		Current_speed_left = 0;
	if(Current_time - Last_speed_check_right >= 800000)
		Current_speed_right = 0;		
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
	ADC_Init();
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
	Call_UART++;
	//Call_GPS++;	
	//Transmit_UART++;
	
}

int main(void)
{
	char GPSstatus[1], GPS_NS[1], GPS_WE[1];
	char GPS_str[80];
	char GPS_symbol_buff;
	uint8_t GPS_str_symbol_index = 0;
	bool GPS_str_is_ready = 0;
	double GPSlatitude = 0, GPSlongitude = 0, GPS_spd = 0, GPS_hdg = 0;

	
	sei();//разрешаем глобальные прерывания
	//cli();//запрет глобальных прерываний
	
	uint32_t Last_speed_check_right = 0, Last_speed_check_left = 0;
	float Current_speed_left = 0;
	float Current_speed_right = 0;
	
	//float kp = 0, ki = 0;
	
	float Raw_gyro_X_Y_Z_values[] = {0, 0, 0}, Real_gyro_X_Y_Z_values[] = {0, 0, 0};
	float Omega_LR_required[] = {0, 0};
	float Radius = -0.4, Velocity = 0.1;
	bool straight = 0;
	char Gyro_data_for_UART[20], Float_to_char_buffer[10], UART_buf[60];
	//PI_regulators Regulator_right(0.35, 0.0085);
	//PI_regulators Regulator_left(0.357, 0.0852);
		
	//Regulator_left.Apply_regulator(0.0, 0.0);	//инициализация регулятора
	//Regulator_right.Apply_regulator(0.0, 0.0);	//инициализация регулятора
	
	Init_all();
	
    while (1) 
    {
		//*************************Чтение данных с гироскопа**************************
		Read_RawValue(Raw_gyro_X_Y_Z_values);
		for (int i=0; i<=2; i++)
		{
			Real_gyro_X_Y_Z_values[i] = Raw_gyro_X_Y_Z_values[i]/939.6544;//пересчет сырых данных в реальные Raw_gyro_X_Y_Z_values[i]/16.4/57.296, где 57.296 - пересчет в рад/с
		}
		//kp = static_cast<float>(ADC_convert(0))/500;
		//ki = static_cast<float>(ADC_convert(1))/1000;
		//dtostrf(Real_gyro_X_Y_Z_values[2], 3, 2, Float_to_char_buffer);//Преобразуем float в char[] чтобы передать по UART
		//sprintf(Gyro_data_for_UART, " %s ", Float_to_char_buffer);//Формируем красивую строку для передачи по UART
		//****************************************************************************
		
		//*************************Анализ данных с GPS********************************
		if(GPS_str_is_ready == true)
		{
			parseGPS(GPS_str, GPSstatus, GPSlatitude, GPS_NS, GPSlongitude, GPS_WE, GPS_spd, GPS_hdg);
			//UART_send_Str(static_cast<int>(GPSlatitude));	
			//UART_send_char('\n');
			GPS_str_is_ready = 0;
		}
		//****************************************************************************
		
		//*************************Вычисление сеоростей*******************************
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
		//*****************************************************************************
		
		//Применяем регулирование
		if(Call_PI_reg >= How_often_call_PI_reg)
		{
			Turn_control(Radius, Velocity, Real_gyro_X_Y_Z_values[2], Omega_LR_required, straight, UART_buf);
			OCR0B = static_cast<uint8_t>(limiter(Min_output_for_Reg, Max_output_for_Reg,Apply_regulator_right(Current_speed_right, Omega_LR_required[1], 0.51, 0.15))); //0.187, 0.0712 0.208, 0.098
			OCR0A = static_cast<uint8_t>(limiter(Min_output_for_Reg, Max_output_for_Reg,Apply_regulator_left(Current_speed_left, Omega_LR_required[0], 0.51, 0.09)));  //0.207, 0.0792 0.208, 0.098
			
			Call_PI_reg = 0;
		}
		
		//Приме данных с GPS по UART и формирование сторки
		if(Call_UART >= How_often_call_UART)
		{
			/*GPS_symbol_buff = UART_get_char();
			if (GPS_symbol_buff == '$')
			{
				GPS_str[GPS_str_symbol_index] = GPS_symbol_buff;
				while(GPS_symbol_buff != '\n')
				{
					GPS_symbol_buff = UART_get_char();
					GPS_str_symbol_index++;
					GPS_str[GPS_str_symbol_index] = GPS_symbol_buff;
					//UART_send_char(GPS_symbol_buff);
				}
				GPS_str[GPS_str_symbol_index+1] = '\0';
				GPS_str_symbol_index = 0;
				GPS_str_is_ready = true;
			}		*/
				Call_UART = 0;
		}
		
		/*if(Transmit_UART == How_often_Transmit_UART)
		{
			
			UART_send_Str(static_cast<int>(GPSlatitude));
			UART_send_char('\n');
			Transmit_UART = 0;
		}*/
    }
}

