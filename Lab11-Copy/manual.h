/*
 * manual.h
 *
 *  Created on: May 1, 2026
 *      Author: ikcorm
 */

#ifndef MANUAL_H_
#define MANUAL_H_

#include "open_interface.h"


typedef struct {
        int angle;
        uint16_t IR;
        float ping;
} ScanPoint;

extern ScanPoint scanPointArray[90];

void logScan();

void man_drive(oi_t *sensor_data);



#endif /* MANUAL_H_ */
