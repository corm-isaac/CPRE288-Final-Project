/*
 * path.h
 *
 *  Created on: May 3, 2026
 *      Author: ikcorm
 */

#ifndef PATH_H_
#define PATH_H_
#include "open_interface.h"

void auto_drive(oi_t *sensor);

void auto_orient(float cybot_heading);

void auto_data_least_objects();

void auto_scan(oi_t *sensor);

#endif /* PATH_H_ */
