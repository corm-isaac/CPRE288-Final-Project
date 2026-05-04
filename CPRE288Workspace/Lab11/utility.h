/*
 * utility.h
 *
 *  Created on: Mar 24, 2026
 *      Author: isaac
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <math.h>

// ---- Constants ----
#define COUNT               90      // Number of scan points
#define RAW_IR_THRESHOLD    10      // IR spike/smoothing threshold
#define EDGE_SCALE          1.5f    // Edge detection scaling factor

// ---- Structs ----

typedef struct {
    int   angle;   // Angle in degrees
    int   IR;      // IR sensor reading (cm)
    float ping;    // Ping sensor reading (cm)
} ScanPoint;

typedef struct {
    int   primary_id;      // Object ID
    int   start_angle;     // Start angle (degrees)
    int   end_angle;       // End angle (degrees)
    int   middle_angle;    // Middle angle (degrees)
    float distance_cm;     // Distance from ping sensor (cm)
    int   radial_width;    // Angular width (degrees)
} Obstacle;

// ---- Global Variables ----

extern char       STOP_BYTE;
extern ScanPoint  scanPointArray[90];
extern Obstacle   objectArray[30];

// ---- Function Prototypes ----

// Lab 11 - Object Detection
void initalizeObject(int objectId, int startAngle, int endAngle, float distance_cm);
int  objectDetermination(void);
void printObjects(int num);
void cleanGlobals();

// Data Cleaning
void clean_data(int data[], int output[]);

// Edge Detection
int  edge_detection(Obstacle obstacles[], int data[], float ping_data[]);
int  find_smallest_object(Obstacle obstacles[], int numObstacles, float ping_data[]);
void print_obstacles(Obstacle obstacles[], int numObstacles, float ping_data[]);

#endif /* UTILITY_H_ */
