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
void Servo1(uint8_t invertido, uint16_t presc);
void updateDutyCycle_servo1(uint8_t duty0); 

#endif /* PWMTIMER0_H_ */