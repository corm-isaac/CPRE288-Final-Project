
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
#include <string.h>
#include "cyBot_Scan.h"
#include <math.h>
#include "uart-interrupt.h"
#include "adc.h"

#ifndef SCAN_H_
#define SCAN_H_

//global vars
extern int angle_array; // Declaration (no memory allocated)
extern float dist_array; // Declaration (no memory allocated)

typedef struct {
    int number;
    int startAngle;
    int endAngle;
    int middleAngle;
    float distance;
    int radialWidth;
    float actualWidth;
} field_object;


void getScan(char mode);

void findActualWidth(field_object *obj);

int objectInfo(field_object *obj, int effective_range, int tolerance);

void printObjectInfo(field_object *obj, int num);

int smallestObjectDegrees(field_object *obj, int num);

void pointPING(int angle);

#endif

