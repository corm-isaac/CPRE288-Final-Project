void getScan();

#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
#include <string.h>
#include "cyBot_Scan.h"

//global vars
int angle_array[90];
float dist_array[90];

typedef struct {
    int number;
    int startAngle;
    int endAngle;
    int middleAngle;
    float distance;
    int radialWidth;
} field_object;


void getScan() {
    cyBOT_init_Scan(0b0111);
    cyBOT_Scan_t scan;

    right_calibration_value = 253750;
    left_calibration_value = 1240750;


    char data[60];
    int range = 180;
    int index = 0;

    int angle;
    for (angle = 0; angle <= range; angle += 2){
        cyBOT_Scan(angle, &scan);

        angle_array[index] = angle;
        dist_array[index] = scan.sound_dist;

        sprintf(data, "Angle: %d\t Distance: %.2f \r\n", angle_array[index], dist_array[index]);
        uart_sendStr(data);

        index++;
    }
}


int objectInfo(field_object *obj) {

    int i = 0;
    int j = 0;

    for (j = 0; j < 90; j++) {
        float distance = dist_array[j];
        float baseDistance = distance;
        int angle = angle_array[j];

        if (distance < 150) {
            obj[i].number = i + 1;
            obj[i].startAngle = angle;
            obj[i].distance = distance;

            while (j < 90 && dist_array[j] > baseDistance - 6 && dist_array[j] < baseDistance + 6) {
                j++;
            }

            obj[i].endAngle = angle_array[j-1];
            obj[i].radialWidth = obj[i].endAngle - obj[i].startAngle;

            if (obj[i].radialWidth > 2) {
                obj[i].middleAngle = (obj[i].endAngle + obj[i].startAngle) / 2;
                i++;
            }
        }
    }

    return i;
}


void printObjectInfo(field_object *obj, int num) {

    char message[80];
    int i;

    for (i = 0; i < num; i++) {
        sprintf(message, "Object %d - Angle: %d, Distance: %.2f, Width: %d\r\n", obj[i].number, obj[i].middleAngle, obj[i].distance, obj[i].radialWidth);
        uart_sendStr(message);
    }
}

int smallestObjectDegrees(field_object *obj, int num) {

    int i;
    int smallestIndex = 0;
    int smallestWidth = obj[0].radialWidth;

    for (i = 1; i < num; i++) {
        if (obj[i].radialWidth < smallestWidth) {
            smallestWidth = obj[i].radialWidth;
            smallestIndex = i;
        }
    }

    return obj[smallestIndex].middleAngle;
}

