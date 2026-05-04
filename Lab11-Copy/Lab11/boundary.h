/*
 * boundary.h
 *
 *  Created on: Apr 22, 2026
 *      Author: ikcorm
 */

#ifndef BOUNDARY_H_
#define BOUNDARY_H_

#include "open_interface.h"

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

uint32_t getFrontRightCliffSensor(oi_t *sensor_data);

uint32_t getFrontLeftCliffSensor(oi_t *sensor_data);

uint32_t getRightCliffSensor(oi_t *sensor_data);

uint32_t getLeftCliffSensor(oi_t *sensor_data);

void printValues(oi_t *sensor_data);

int checkBoundary(oi_t *sensor_data);

#endif /* BOUNDARY_H_ */
