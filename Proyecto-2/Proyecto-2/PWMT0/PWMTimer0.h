/*
 * PWMTimer0.h
 *
 * Created: 4/22/2025 11:05:19 AM
 *  Author: razer
 */ 


#ifndef PWMTIMER0_H_
#define PWMTIMER0_H_

#include <avr/io.h>
#define invt 1
#define no_invt 0
void PWM0A(uint8_t invertido, uint16_t presc); //cambiar nombres
void updateDutyCycle(uint8_t duty0); //cambiar nombres

#endif /* PWMTIMER0_H_ */