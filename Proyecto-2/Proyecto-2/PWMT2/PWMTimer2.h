/*
 * PWMTimer2.h
 *
 * Created: 4/22/2025 11:07:09 AM
 *  Author: razer
 */ 


#ifndef PWMTIMER2_H_
#define PWMTIMER2_H_

#include <avr/io.h>
#define invt2 1
#define no_invt2 0

void PWM2A(uint8_t invertido2, uint16_t presc2); //cambiar nombres
void updateDutyCycle2(uint8_t duty2); //cambiar nombres 

void PWM2B(uint8_t invertido2); // NUEVO
void updateDutyCycle2B(uint8_t duty2); // NUEVO

#endif /* PWMTIMER2_H_ */