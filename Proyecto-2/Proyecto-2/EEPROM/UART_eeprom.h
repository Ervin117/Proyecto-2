/*
 * UART_eeprom.h
 *
 * Created: 5/4/2025 2:40:42 PM
 *  Author: razer
 */ 


#ifndef UART_EEPROM_H_
#define UART_EEPROM_H_

#include <avr/io.h>
void serialUART();
void serialLECT(char letra);
void stringTermi(char* oracion); 
void valor360(uint16_t valor, char*oracion); //Transformar el string a un int


#endif /* UART_EEPROM_H_ */