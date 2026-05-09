/*
 * @file boundary.h
 * @brief Header file for boundary.c
 *
 * @author Isaac Cormier, Mila Haynes
 *
 * @date 04/22/26
 */

#ifndef BOUNDARY_H_
#define BOUNDARY_H_

#include "open_interface.h"
#include "log_message.h"

extern const uint32_t FRONT_LEFT_CONCRETE;
extern const uint32_t LEFT_CONCRETE;
extern const uint32_t FRONT_RIGHT_CONCRETE;
extern const uint32_t RIGHT_CONCRETE;

extern const uint32_t FRONT_LEFT_TAPE;
extern const uint32_t LEFT_TAPE;
extern const uint32_t FRONT_RIGHT_TAPE;
extern const uint32_t RIGHT_TAPE;

extern const uint32_t FRONT_LEFT_HOLE;
extern const uint32_t LEFT_HOLE;
extern const uint32_t FRONT_RIGHT_HOLE;
extern const uint32_t RIGHT_HOLE;

uint32_t get_front_right_cliff_sensor(oi_t *sensor_data);

uint32_t get_front_left_cliff_sensor(oi_t *sensor_data);

uint32_t get_right_cliff_sensor(oi_t *sensor_data);

uint32_t get_left_cliff_sensor(oi_t *sensor_data);

void print_values(oi_t *sensor_data);

int check_boundary(oi_t *sensor_data);

#endif /* BOUNDARY_H_ */
