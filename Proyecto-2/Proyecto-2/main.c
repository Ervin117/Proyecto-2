/*
 * Proyecto-2.c
 *
 * Created: 5/3/2025 4:48:00 PM
 * Author : Ervin Gomez 231226
 
 Proyecto 2: Se presenta el codigo para realizar el
 movimiento de una garra mediante joysticks,
 implementando librerias especializadas para el control
 de las señales PWM, al igual que el uso de el monitor
 serial para realizar comandos especializados y la
 implementación de la EEPROM para movimientos ya preestablecidos.
 */ 


/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PWMT0/PWMTimer0.h"
#include "PWMT1/PWMTimer1.h"
#include "PWMT2/PWMTimer2.h"
#include "EEPROM/UART_eeprom.h"

//variables globales. 
uint8_t modo = 0;		//variable para controlar los modos 
uint16_t duty2 = 0;		//servo 2
uint16_t duty = 0;		//servo 1
uint16_t duty0 = 0;		//servo 3
uint16_t duty1 = 0;		//servo 4
uint16_t addres = 0x00; //Dirección inicial de la EEPROM
uint8_t act = 0;		//Varible para guardar paquetes de datos. 
uint8_t prom = 0;		//Variable para guardar la catidad de paquetes
uint8_t input_index = 0;
#define MAX_CHAR 20
char input_joy[MAX_CHAR]; 
/****************************************/
// Function prototypes
void setup();
void conf_ADC();			//Configuración del ADC
void ADC_mod();				//Modo manual 
void EEPROM_mod_read();		// Modo para reproducir posiciones
void UART_mod();			// Modo para controlar desde la compu 

void writeEEPROM(uint8_t dato, uint16_t direcc); //Configuración para escribir en la EEPROM
uint8_t readEEPROM(uint16_t direcc);			 //Configuración para leer la EEPROM

/****************************************/
// Main Function
int main(void)
{
	setup();
	conf_ADC();
	serialUART(); 
	
	while (1)
	{
		switch(modo)
		{
			case 0: 
				ADC_mod(); 
				break; 
			case 1:
				EEPROM_mod_read(); 
				break; 
			case 2: 
				UART_mod(); 
				break; 
		}
	}
}

/****************************************/
// NON-Interrupt subroutines
void setup()
{
	cli();
	//CLKPR = (1<<CLKPCE);
	//CLKPR = (1<<CLKPS2);
	
	//Configuración de las salidas de los modos. 
	DDRD |= (1 << DDD2);
	PORTD &= ~(1 << PORTD2);
	DDRD |= (1 << DDD3);
	PORTD &= ~(1 << PORTD3);
	
	//Configuración de los botones. 
	DDRC &= ~((1<<DDC0) | (1<<DDC1));
	PORTC |= (1 << PORTC0) | (1 << PORTC1);
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1 << PCINT9) | (1 << PCINT8);
	
	//Configuración de los servos. 
	Servo1(no_invt, 1024); 
	Servo2(no_invt, 8); 
	Servo4(no_invt2, 1024); 
	Servo3(no_invt); 
	
	UCSR0B = 0;
	sei();
}

//Configuracion inicial del ADC
void conf_ADC()
{
	ADMUX = 0;
	ADMUX |= (1<<REFS0) | (1 << MUX0) |  (1 << MUX1) | (1<<MUX2); //| (1<<ADLAR)
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADPS1) | (1<<ADPS2) |  (1<<ADIE) | (1<<ADEN);
	ADCSRA |= (1<<ADSC);
}

void ADC_mod()
{
	ADCSRA |=(1<<ADIE); 
	ADCSRA |= (1<<ADSC);
	PORTD &= ~(1<<PORTD2);
	PORTD &= ~(1<<PORTD3);
	
	//Solo se guarda si se presiona el botón. 
	if (act	== 1 && prom <= 5)
	{
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD2);
		writeEEPROM(OCR0A, addres++);
		writeEEPROM(OCR1A, addres++);
		writeEEPROM(OCR1B, addres++);
		writeEEPROM(OCR2A, addres++);
		prom++;
		act = 0; //setear el valor de los paquetes guardados
	}

}

//Modo para leer la eeprom
void EEPROM_mod_read()
{
	ADCSRA &= ~(1<<ADIE);
	PORTD &= ~(1<<PORTD2);
	PORTD |= (1<<PORTD3);
	
	//Ejecuta la primera secuencia
	if (act >= 1 && act <= 4 && act <= prom)
	{	
		uint16_t add = (act-1)*4; 
		OCR0A = readEEPROM(add);
		OCR1A = readEEPROM(add+1);//+1
		OCR1B = readEEPROM(add+2);//+2
		OCR2A = readEEPROM(add+3);//+3
		
	}
}

void UART_mod()
{
	ADCSRA |= (1<<ADIE);
	PORTD |= (1<<PORTD2);
	PORTD &= ~(1<<PORTD3); 
	 
}

void writeEEPROM(uint8_t dato, uint16_t direcc)
{
	while (EECR &(1<<EEPE)); 
	EEAR = direcc; 
	EEDR = dato; 
	EECR |= (1<<EEMPE); 
	EECR |= (1<<EEPE); 
	
}

uint8_t readEEPROM(uint16_t direcc)
{
	while (EECR &(1<<EEPE));
	EEAR = direcc;
	EECR |= (1<<EERE);
	return EEDR; 
	
}

/****************************************/
// Interrupt routines
ISR(ADC_vect)
{
	uint8_t currentADC = ADMUX & 0x07;
	uint16_t temp = ADC;
	if (currentADC == 7)
	{
		duty = (1.96*temp)+2000.46;		//servo 1 //timer 1
		updateDutyCycle_servo2(duty);
		ADMUX = 0;
		ADMUX |= (1<<REFS0);			// | (1<<ADLAR);
		ADMUX |= (1 << MUX1) | (1<<MUX2);
	}
	else if (currentADC == 6)
	{
		duty2 = (0.0153*temp)+15.62;	//servo 2
		updateDutyCycle_servo4(duty2);
		ADMUX = 0;
		ADMUX |= (1<<REFS0);			// | (1<<ADLAR);
		ADMUX |= (1 << MUX0) | (1<<MUX2);
	}
	else if (currentADC == 5)
	{
		duty0 = (0.0153*temp)+15.62;	//servo 3
		updateDutyCycle_servo1(duty0);
		ADMUX = 0;
		ADMUX |= (1<<REFS0);			// | (1<<ADLAR);
		ADMUX |= (1<<MUX2);
	}
	else if (currentADC == 4)
	{
		duty1 = (1.96*temp)+2000.46;	//servo 4 //timer1
		updateDutyCycle_servo3(duty1);
		ADMUX = 0;
		ADMUX |= (1<<REFS0);			// | (1<<ADLAR);
		ADMUX |= (1 << MUX0) |  (1 << MUX1) | (1<<MUX2);
	}
	ADCSRA |= (1<<ADSC);
}

ISR(PCINT1_vect)
{
	//Boton para cambio de modo.
	if (!(PINC & (1<<PINC0)))
	{
		modo++;
		if (modo == 3)
		{
			modo = 0;
		}
	}
	//Boton para acción de guardado
	else if (!(PINC & (1<<PINC1)))
	{
		act++;
		if (act == 5)
		{
			act = 0;
		}
	}
}

ISR(USART_RX_vect)
{	
	char uar = UDR0;
	if (modo == 2) {
		if (uar == '\n' || uar == '\r') {
			input_joy[input_index] = '\0';
			uint16_t val_adc[4] = {0};
			uint8_t servo_index = 0;
			uint16_t val_act = 0; 

			for (uint8_t i = 0; i <= input_index && servo_index < 4; i++) {
				char c = input_joy[i];
				if (c >= '0' && c <= '9') { 
					val_act = val_act * 10 + (c - '0');
				}
				else if (c == ',' || c == '\0') {
					if (val_act > 1023) val_act = 1023;
					val_adc[servo_index++] = val_act;
					val_act = 0;
				}
			}

			if (servo_index == 4) {
				OCR1A = (uint16_t)((1.96 * val_adc[0]) + 2000.46);
				OCR1B = (uint16_t)((0.0153 * val_adc[1]) + 15.62);  
				OCR0A = (uint16_t)((0.0153 * val_adc[2]) + 15.62);
				OCR2A = (uint16_t)((1.96 * val_adc[3]) + 2000.46);

				char str_val[6];
				for (uint8_t j = 0; j < 4; j++) {
					valor360(val_adc[j], str_val); 
					stringTermi(str_val);
					if (j < 3) serialLECT(','); 
				}
				serialLECT('\n');
			}
			input_index = 0;
		}
		else if (input_index < MAX_CHAR - 1) {
			input_joy[input_index++] = uar;
		}
	}
}