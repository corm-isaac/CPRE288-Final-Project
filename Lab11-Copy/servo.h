/*
 * servo.h
 *
 *  Created on: Apr 7, 2026
 *      Author: cooperrs
 */

#ifndef SERVO_H_
#define SERVO_H_


#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "button.h"


void servo_init(void);

void servo_move(float degrees);

// init lcd, button, and timer before calling
void servo_calibrate(void);

uint16_t IRScan(float degrees);

float PingScan(int degree);

#endif /* SERVO_H_ */
