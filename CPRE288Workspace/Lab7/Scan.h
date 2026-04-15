
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
#include <string.h>
#include "cyBot_Scan.h"
#include <math.h>
#include "uart-interrupt.h"
//global vars
int angle_array[90];
float dist_array[90];
int ir_array[90];
int small_object_index = 0;

typedef struct {
    int number;
    int startAngle;
    int endAngle;
    int middleAngle;
    float distance; //stores PING
    float ir_distance;
    int radialWidth;
    float actualWidth;
} field_object;


void getScan(char mode) {
    cyBOT_init_Scan(0b0111);
    cyBOT_Scan_t scan;

    right_calibration_value = 285250;
    left_calibration_value = 1193500;


    uart_sendStr("\r\nBeginning Scan!\r\n");

    char data[60]; //data buffer
    int range = 180;
    int index = 0;

    int tol = 2;
    int value = 0;

    int n;
    int angle;
    for (angle = 0; angle <= range; angle += 2){
        cyBOT_Scan(angle, &scan);

        angle_array[index] = angle;
        dist_array[index] = scan.sound_dist;

        if(mode){ //if mode is truthy, overwrites PING data w/ IR (yoinked from that other method)
            for(n=0; n < tol; n++){
                cyBOT_Scan(angle, &scan);
                value += scan.IR_raw_val;
            }
            ir_array[index] = (int)(value/tol);
            value = 0;
        }
        sprintf(data, "Angle: %d\t Distance: %.2f IR: %d \r\n", angle_array[index], dist_array[index], ir_array[index]);
        uart_sendStr(data);
        index++;
    }
    uart_sendStr("End of Scan!\r\n");

}

void findActualWidth(field_object *obj){
    obj->actualWidth= 2.0 * M_PI * (obj->distance) * ((obj->middleAngle) / 360);
}

int objectInfo(field_object *obj, int IR_range, int tolerance) { //MODIFIED FOR IRRRRRRRR
    char data[100];

    int i = 0; //objects
    int j;

    for (j = 0; j < 90; j++) {
        float distance = ir_array[j];
        float baseDistance = distance;
        char startJ = j;
        char endJ;


        if (distance > IR_range) { //modified for IR (direction of sign changed)
            while (j < 90 && distance > baseDistance - tolerance && distance < baseDistance + tolerance) {
                baseDistance = distance;
                j++;
                distance = ir_array[j];

            }
            endJ = j;
            //float culprit = distance;   // distance after loop = value that broke tolerance

            if ((endJ - startJ) <= 2) { //anything with 4 or less degrees is hopefully ignored
                uart_sendStr(data);
                continue;
            }

            //sprintf(data, "", )
            uart_sendStr("Object Display: \r\n");

            obj[i].number = i + 1;
            obj[i].startAngle = angle_array[startJ];
            obj[i].ir_distance = baseDistance;
            obj[i].distance = dist_array[j];

            //math to find radial Width and middle Angle
            obj[i].endAngle = angle_array[j-1];
            obj[i].radialWidth = obj[i].endAngle - obj[i].startAngle;

            //sprintf(data, "Object Num=%d, StartJ=%d, endJ=%d, baseDist=%f\r\n", obj[i].number, startJ, endJ, baseDistance);
            //uart_sendStr(data);

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
        sprintf(message, "Object %d - Angle: %d, Distance: %.2f, Rad Width: %d\r\n", obj[i].number, obj[i].middleAngle, obj[i].distance, obj[i].radialWidth);
        uart_sendStr(message);
    }
}

int smallestObjectDegrees(field_object *obj, int num) {
    //cyBOT_Scan_t pscan;
    int i;
    int smallestIndex = 0;
    int smallestWidth = obj[0].radialWidth;

    for (i = 1; i < num; i++) {
        if (obj[i].radialWidth < smallestWidth) {
            smallestWidth = obj[i].radialWidth;
            smallestIndex = i;
            small_object_index = i;
        }
    }
    return obj[smallestIndex].middleAngle;
}



void pointPING(int angle) {
    cyBOT_Scan_t pscan;
    //int smallestIndex = 0;
//    int smallestWidth = obj[0].radialWidth;
    char message[20];

    cyBOT_Scan(angle, &pscan);
    sprintf(message, "POINT PING OBJECT: Angle: %d, Distance: %.2f\r\n", angle, pscan.sound_dist);
    uart_sendStr(message);

}



