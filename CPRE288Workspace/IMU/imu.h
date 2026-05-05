/*
 * imu.h
 *
 *  Created on: Apr 22, 2026
 *      Author: mifarmer
 */

#ifndef IMU_H_
#define IMU_H_

#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"


void imu_init();
void imu_write_command(char address, char data);
float imu_get_heading_deg();




#endif /* IMU_H_ */
