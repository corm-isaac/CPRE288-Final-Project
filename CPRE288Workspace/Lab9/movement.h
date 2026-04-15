/*
 * movement.h
 *
 *  Created on: Feb 11, 2026
 *      Author: ikcorm
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"



double move_forward(oi_t *sensor_data, double distance_mm);

double turn_right(oi_t *sensor_data, double degrees);

double turn_left(oi_t *sensor_data, double degrees);

void bumpLeft(oi_t *sensor_data);

void bumpRight(oi_t *sensor_data);

#endif /* MOVEMENT_H_ */
