/*
 * @file movement.h
 * @brief Header file for movement.c
 *
 * @author Michael Farmer, Isaac Cormier, Mila Haynes, Cooper Sanders
 *
 * @date 02/04/2026
 */

#include "movement.h"
#include "boundary.h"
#include "log_message.h"
#include "utility.h"
#include "open_interface.h"

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

double move_forward_mm(oi_t *sensor_data, double distance_mm);
double move_backward_mm(oi_t *sensor_data, double distance_mm);
double turn_right_deg(oi_t *sensor_data, double degrees);
double turn_left_deg(oi_t *sensor_data, double degrees);

#endif /* MOVEMENT_H_ */
