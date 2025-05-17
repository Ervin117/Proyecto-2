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
uint16_t add = 0x00;	//Dirección para operación 
/****************************************/
// Function prototypes
void setup();
void conf_ADC();			//Configuración del ADC
void ADC_mod();				//Modo manual 
void EEPROM_mod_write();	//Modo para guaradar posiciones
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
	conf_ADC();	
	PORTD &= ~(1<<PORTD2);
	PORTD &= ~(1<<PORTD3);
	
	//Solo se guarda si se presiona el botón. 
	if (act	== 1 && prom <= 5)
	{
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD2);
		writeEEPROM(duty, addres++);
		writeEEPROM(duty0, addres++);
		writeEEPROM(duty1, addres++);
		writeEEPROM(duty2, addres++);
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
	if (act == 1)
	{	
		addres = 0x00;
		OCR0A = readEEPROM(addres++);
		OCR1A = readEEPROM(addres++);
		OCR1B = readEEPROM(addres++);
		OCR2A = readEEPROM(addres++);
		
	}
	//Ejecuta la segunda secuencia
	if (act == 2)
	{
		addres = 0x00;
		add = addres + 4; 
		OCR0A = readEEPROM(add++);
		OCR1A = readEEPROM(add++);
		OCR1B = readEEPROM(add++);
		OCR2A = readEEPROM(add++);
	}
	//Ejecuta la tercera secuencia 
	if (act == 3)
	{
		addres = 0x00;
		add = addres + 8;
		OCR0A = readEEPROM(add++);
		OCR1A = readEEPROM(add++);
		OCR1B = readEEPROM(add++);
		OCR2A = readEEPROM(add++);
	}
	//Ejecuta la cuarta secuencia
	if (act == 4 )
	{
		addres = 0x00;
		add = addres + 12;
		OCR0A = readEEPROM(add++);
		OCR1A = readEEPROM(add++);
		OCR1B = readEEPROM(add++);
		OCR2A = readEEPROM(add++);
	}
}

void UART_mod()
{
	ADCSRA &= ~(1<<ADIE);
	PORTD |= (1<<PORTD2);
	PORTD &= ~(1<<PORTD3); 
	
	//desactivar las interrupciones
	//meter los valores del la escritura de la EEPROM. 
}

void writeEEPROM(uint8_t dato, uint16_t direcc)
{
	while (EEAR &(1<<EEPE)); 
	EEAR = direcc; 
	EEDR = dato; 
	EECR |= (1<<EEMPE); 
	EECR |= (1<<EEPE); 
	
}

uint8_t readEEPROM(uint16_t direcc)
{
	while (EEAR &(1<<EEPE));
	EEAR = direcc;
	EECR |= (1<<EEMPE);
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
		duty = (1.96*temp)+2000.46; //servo 1 //timer 1
		updateDutyCycle_servo2(duty);
		ADMUX = 0;
		ADMUX |= (1<<REFS0);// | (1<<ADLAR);
		ADMUX |= (1 << MUX1) | (1<<MUX2);
	}
	else if (currentADC == 6)
	{
		duty2 = (0.0153*temp)+15.62; //servo 2
		updateDutyCycle_servo4(duty2);
		ADMUX = 0;
		ADMUX |= (1<<REFS0);// | (1<<ADLAR);
		ADMUX |= (1 << MUX0) | (1<<MUX2);
	}
	else if (currentADC == 5)
	{
		duty0 = (0.0153*temp)+15.62; //servo 3
		updateDutyCycle_servo1(duty0);
		ADMUX = 0;
		ADMUX |= (1<<REFS0);// | (1<<ADLAR);
		ADMUX |= (1<<MUX2);
	}
	else if (currentADC == 4)
	{
		duty1 = (1.96*temp)+2000.46;  //servo 4 //timer1
		updateDutyCycle_servo3(duty1);
		ADMUX = 0;
		ADMUX |= (1<<REFS0);// | (1<<ADLAR);
		ADMUX |= (1 << MUX0) |  (1 << MUX1) | (1<<MUX2);
	}
	ADCSRA |= (1<<ADSC);
}
/*
#define MAX_BUFFER_UART 32

ISR(USART_RX_vect)
{

		char buffer_uart[MAX_BUFFER_UART];
		uint8_t index_uart = 0;

		if (modo != 2) return; // Solo ejecutar en modo UART


		if (UDR0 == '\n' || UDR0 == '\r') {
			buffer_uart[index_uart] = '\0'; // Terminar la cadena

			uint16_t valores_servo[4] = {0};
			uint8_t indice_valor = 0;
			uint16_t valor_temporal = 0;

			for (uint8_t i = 0; buffer_uart[i] != '\0'; i++) {
				char caracter_actual = buffer_uart[i];

				if (caracter_actual >= '0' && caracter_actual <= '9') {
					valor_temporal = valor_temporal * 10 + (caracter_actual - '0');
					} else if (caracter_actual == ',') {
					if (indice_valor < 4) {
						if (valor_temporal > 180) valor_temporal = 180; // Limitar a 180 grados
						valores_servo[indice_valor++] = valor_temporal;
						valor_temporal = 0;
					}
				
				// Control de los cuatro servos de 360 grados
				for (uint8_t i = 0; i < 4; i++) {
					uint16_t valor_recibido = valores_servo[i];
					uint16_t duty_ciclo;

					if (valor_recibido < (VALOR_STOP_SERVO - RANGO_VELOCIDAD)) {
						duty_ciclo = map(valor_recibido, 0, VALOR_STOP_SERVO - RANGO_VELOCIDAD, SERVO_MAX, SERVO_STOP);
						} else if (valor_recibido > (VALOR_STOP_SERVO + RANGO_VELOCIDAD)) {
						duty_ciclo = map(valor_recibido, VALOR_STOP_SERVO + RANGO_VELOCIDAD, 180, SERVO_STOP, SERVO_MIN);
						} else {
						duty_ciclo = SERVO_STOP;
					}

					// Asignar el duty cycle al registro OCR correspondiente
					if (i == 0) OCR1B = duty_ciclo; // Servo 1
					else if (i == 1) OCR1A = duty_ciclo; // Servo 2
					else if (i == 2) OCR0A = duty_ciclo; // Servo 3
					else if (i == 3) OCR0B = duty_ciclo; // Servo 4
				}

				index_uart = 0; // Reiniciar el buffer
				} else {
				if (index_uart < MAX_BUFFER_UART - 1) {
					buffer_uart[index_uart++] = UDR0;
				}
			}
		}
}
*/
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