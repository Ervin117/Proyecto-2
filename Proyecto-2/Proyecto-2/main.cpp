/*
 * Proyecto-2.cpp
 *
 * Created: 4/22/2025 10:46:28 AM
 * Author : Ervin Gomez 231226
 
 Proyecto 2: Se presenta el codigo para realizar el 
 movimiento de un () mediante potenciometros, 
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

uint8_t duty2 = 0;
uint16_t duty = 0;
uint8_t duty0 = 0; 
/****************************************/
// Function prototypes
void setup();
void conf_ADC(); 

/****************************************/
// Main Function
int main(void)
{
	setup(); 
	conf_ADC(); 
	
	while (1)
	{
		updateDutyCycle1(duty);//se actualiza los valores de los servos. 
		updateDutyCycle2(duty2);
		updateDutyCycle(duty0);
		_delay_ms(1);
	}
}

/****************************************/
// NON-Interrupt subroutines
void setup()
{
	cli(); 
	CLKPR = (1<<CLKPCE);
	CLKPR = (1<<CLKPS2);
	PWM0A(no_invt, 64); //cambiar nombres
	PWM1A(no_invt, 64); //cambiar nombres
	PWM2A(no_invt2, 64); //cambiar nombres 
	
	//UCSR0B = 0;
	sei();
}
void conf_ADC()
{
	ADMUX = 0;
	ADMUX |= (1<<REFS0) | (1<<ADLAR) | (1 << MUX0) |  (1 << MUX1) | (1<<MUX2); //
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADPS1) | (1<<ADPS2) |  (1<<ADIE) | (1<<ADEN);
	ADCSRA |= (1<<ADSC);
}
/****************************************/
// Interrupt routines
ISR(ADC_vect)
{
	uint8_t currentADC = ADMUX & 0x07;
	uint8_t temp = ADCH;
	if (currentADC == 7)
	{
		duty = 8 + (temp*(38-8)/255);
		ADMUX = 0;
		ADMUX |= (1<<REFS0) | (1<<ADLAR);
		ADMUX |= (1 << MUX1) | (1<<MUX2);
	}
	else if (currentADC == 6)
	{
		duty2 = 7 + (temp*(37-7)/255);
		ADMUX = 0;
		ADMUX |= (1<<REFS0) | (1<<ADLAR);
		ADMUX |= (1 << MUX0) | (1<<MUX2);
	}
	else if (currentADC == 5) //cambiar 
	{
		duty0 = 7 + (temp*(37-7)/255);
		ADMUX = 0;
		ADMUX |= (1<<REFS0) | (1<<ADLAR);
		ADMUX |= (1 << MUX0) |  (1 << MUX1) | (1<<MUX2);
	}
	ADCSRA |= (1<<ADSC);
}