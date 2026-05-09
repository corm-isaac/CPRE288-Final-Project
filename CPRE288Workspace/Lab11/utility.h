/*
 * @file utility.h
 * @brief random code that is too cool to fit elsewhere. In this instance it's mostly the object detections
 * logic.
 *
 *  @author Cooper Sanders, Isaac Cormier
 *
 *  @date 03/24/2026
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <math.h>

// ---- Constants ----
#define COUNT 90            // Number of scan points
#define RAW_IR_THRESHOLD 10 // IR spike/smoothing threshold
#define EDGE_SCALE 1.5f     // Edge detection scaling factor

// ---- Structs ----

typedef struct
{
    int angle;  // Angle in degrees
    int IR;     // IR sensor reading (cm)
    float ping; // Ping sensor reading (cm)
} scan_point;

typedef struct
{
    int primary_id;    // Object ID
    int start_angle;   // Start angle (degrees)
    int end_angle;     // End angle (degrees)
    int middle_angle;  // Middle angle (degrees)
    float distance_cm; // Distance from ping sensor (cm)
    int radial_width;  // Angular width (degrees)
} Obstacle;

// ---- Global Variables ----

extern char STOP_BYTE;
extern ScanPoint g_scan_point_array[90];
extern Obstacle g_object_array[30];

// ---- Function Prototypes ----

// Lab 11 - Object Detection
void initalize_object(int object_id, int start_angle, int end_angle, float distance_cm);
int object_determination(void);
void print_objects(int num);
void clean_globals();

// Data Cleaning
void clean_data(int data[], int output[]);

// Edge Detection
void suck(int time);
#endif /* UTILITY_H_ */
