/*
 * movement.h
 *
 *  Created on: Feb 4, 2026
 *      Author: mifarmer
 */
#include "open_interface.h"

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

double move_forward(oi_t *sensor_data, double distance_mm);
double move_backward(oi_t *sensor_data, double distance_mm);
double turn_right(oi_t *sensor_data, double degrees);
double turn_left(oi_t *sensor_data, double degrees);
void go_around(oi_t *sensor_data, short direction);
void oops(oi_t *sensor_data, int sensor_tripped);

void bumpLeft(oi_t *sensor_data);
void bumpRight(oi_t *sensor_data);


#endif /* MOVEMENT_H_ */
