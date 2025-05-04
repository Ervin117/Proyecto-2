/*
 * PWMTimer1.h
 *
 * Created: 4/22/2025 11:06:32 AM
 *  Author: razer
 */ 

#ifndef PWMTIMER1_H_
#define PWMTIMER1_H_

#include <avr/io.h>
#define invt 1
#define no_invt 0

void PWM1A(uint8_t invertido, uint16_t presc); //cambiar nombres
void updateDutyCycle1(uint16_t duty); // duty entre 0 y ICR1 
//cambiar nombres

void PWM1B(uint8_t invertido);
void updateDutyCycle1B(uint16_t duty);

#endif /* PWMTIMER1_H_ */