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
void imu_set_compass_mode();
void imu_set_ndof_mode();




#endif /* IMU_H_ */
