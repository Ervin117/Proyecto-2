/*
 * UART_eeprom.c
 *
 * Created: 5/4/2025 2:41:09 PM
 *  Author: razer
 */ 
#include "UART_eeprom.h"

void serialUART()
{
		DDRD |= (1<< DDD1);
		DDRD &= ~(1<<DDD0);
		
		//configurado para enviar y recibir.
		UCSR0A |= (1<<U2X0);
		UCSR0B |= (1<< RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
		UCSR0C |= (1<< UCSZ01) | (1<<UCSZ00);
		UBRR0 = 34;	//valor calculado para los 57600
}

void serialLECT(char letra)
{
	while ((UCSR0A & (1<<UDRE0)) == 0);
	UDR0 = letra;
	
}

void stringTermi(char* oracion)
{
	for (uint8_t i = 0; *(oracion+i) != '\0'; i++)
	{
		serialLECT(oracion[i]);
	}
}

void valor360(uint16_t num, char* str)
{
	   if (num >= 1000) {
		   *str++ = (num / 1000) + '0';  // Ej: 1023 ? '1'
		   num %= 1000;                  // 1023 % 1000 = 23
		   *str++ = (num / 100) + '0';   // 23 / 100 = 0 ? '0'
		   num %= 100;                   // 23 % 100 = 23
		   *str++ = (num / 10) + '0';    // 23 / 10 = 2 ? '2'
		   num %= 10;                    // 23 % 10 = 3
		   *str++ = num + '0';           // '3'
	   }
	   else if (num >= 100) {
		   *str++ = (num / 100) + '0';   // Ej: 512 ? '5'
		   num %= 100;                   // 512 % 100 = 12
		   *str++ = (num / 10) + '0';    // 12 / 10 = 1 ? '1'
		   num %= 10;                    // 12 % 10 = 2
		   *str++ = num + '0';           // '2'
	   }
	   else if (num >= 10) {
		   *str++ = (num / 10) + '0';    // Ej: 45 ? '4'
		   num %= 10;                    // 45 % 10 = 5
		   *str++ = num + '0';           // '5'
	   }
	   else {
		   *str++ = num + '0';           // Ej: 7 ? '7'
	   }
	   *str = '\0';  // Terminador nulo
}