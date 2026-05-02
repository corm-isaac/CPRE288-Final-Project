/*
 * utility.h
 *
 *  Created on: Mar 24, 2026
 *      Author: cooperrs
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <math.h>
#include <stdio.h>
#include "uart-interrupt.h"
#define RAW_IR_THRESHOLD 50
#define COUNT 91
#define EDGE_SCALE 1.3

extern char STOP_BYTE;
typedef struct {
    int start_angle;
    int end_angle;
    float distance_cm;
} Obstacle;

void clean_data(int data[], int output[]);

int edge_detection(Obstacle obstacles[], int data[], float ping_data[]);

int find_smallest_object(Obstacle obstacles[], int numObstacles, float ping_data[]);

void print_obstacles(Obstacle obstacles[], int numObstacles, float ping_data[]);



#endif /* UTILITY_H_ */
