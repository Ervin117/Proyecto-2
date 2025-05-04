/*
 * PWMTimer2.c
 *
 * Created: 4/22/2025 11:07:26 AM
 *  Author: razer
 */ 

#include "PWMTimer2.h"
void PWM2A(uint8_t invertido2, uint16_t presc2) //cambiar nombres
{
	DDRB |=(1<< DDB3);

	TCCR2A &= ~((1<<COM2A1) | (1<<COM2A0));
	if (invertido2 == invt2)
	{
		TCCR2A |=(1<<COM2A1) | (1<<COM2A0); //modo invertido
	}
	else
	{
		TCCR2A |=(1<<COM2A1); //modo no invertido
	}
	//TCCR2B |= (1<< WGM22);
	TCCR2A |= (1<< WGM21) | (1<< WGM20); //Setear el modo fast

	TCCR2B &= ~(1<<CS22) | (1<<CS21) | (1<<CS20);
	switch(presc2){
		case 1:
		TCCR2B |= (1<<CS20);
		break;
		case 8:
		TCCR2B |= (1<<CS21);
		break;
		case 32:
		TCCR2B |= (1<<CS21) | (1<<CS20);
		break;
		case 64:
		TCCR2B |= (1<<CS22);
		break;
		case 128:
		TCCR2B |= (1<<CS22) | (1<<CS20);
		break;
		case 256:
		TCCR2B |= (1<<CS22) | (1<<CS21);
		break;
		case 1024:
		TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
		break;
		default:
		TCCR2B |= (1<<CS22) | (1<<CS21); //256 de prescaler
		break;
	}
}
void updateDutyCycle2(uint8_t duty2) //cambiar nombres
{
	OCR2A = duty2;
}

void PWM2B(uint8_t invertido2)
{
	DDRD |= (1 << DDD3); // OC2B como salida

	TCCR2A &= ~((1 << COM2B1) | (1 << COM2B0)); // limpiar configuración previa

	if (invertido2 == invt2)
	{
		TCCR2A |= (1 << COM2B1) | (1 << COM2B0); // modo invertido
	}
	else
	{
		TCCR2A |= (1 << COM2B1); // modo no invertido
	}
}

void updateDutyCycle2B(uint8_t duty2)
{
	OCR2B = duty2;
}
