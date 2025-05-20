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
		   *str++ = (num / 1000) + '0';  
		   num %= 1000;                  
		   *str++ = (num / 100) + '0';   
		   num %= 100;                   
		   *str++ = (num / 10) + '0';    
		   num %= 10;                    
		   *str++ = num + '0';          
	   }
	   else if (num >= 100) {
		   *str++ = (num / 100) + '0';   
		   num %= 100;                   
		   *str++ = (num / 10) + '0';   
		   num %= 10;                   
		   *str++ = num + '0';           
	   }
	   else if (num >= 10) {
		   *str++ = (num / 10) + '0';    
		   num %= 10;                    
		   *str++ = num + '0';          
	   }
	   else {
		   *str++ = num + '0';           
	   }
	   *str = '\0';  
}