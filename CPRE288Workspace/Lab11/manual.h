/*
 * @file manual.h
 * @brief Header file for manual.c
 *
 * @author Isaac Cormier, Mila Haynes
 *
 * @date 05/01/26
 */

#ifndef MANUAL_H_
#define MANUAL_H_

#include "open_interface.h"
#include "log_message.h"

void log_scan();

void man_drive(oi_t *sensor_data);

#endif /* MANUAL_H_ */
