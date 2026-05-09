/*
 * @file servo.h
 * @brief
 * Intialization and movement and calibration of the servo on the cybot
 * Update: Added scan functions for PING and IR
 * @author Cooper Sanders, Isaac Cormier
 * @date 04/07/26
 *
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

uint16_t IR_scan(float degrees);

float ping_scan(int degree);

#endif /* SERVO_H_ */
