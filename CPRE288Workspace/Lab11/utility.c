/*
 * utility.c
 *
 *  Created on: Mar 24, 2026
 *      Author: cooperrs
 */

#include "utility.h"

/*
 * utility.c
 *
 *  Created on: Mar 24, 2026
 *      Author: cooperrs
 */


#include "Timer.h"
#include "lcd.h"

#include "uart-interrupt.h"
#include "utility.h"
#include "open_interface.h"
#include "movement.h"
#include "adc.h"
#include "ping.h"
#include "button.h"
#include "servo.h"
#include "logMessage.h"
#include "boundary.h"
#include "manual.h"
//GLOBAL VARIABLES - Very pretty unsafe but i dont care
char STOP_BYTE = 0; //Designed to be manual override to stop the bot. not yet implemented 5/2
ScanPoint scanPointArray[90]; //An array of ScanPoint objects - ScanPoint objects store the angle, IR and PING value at each angle increment
Obstacle objectArray[30]; //An array of objects that we have

void logScan(){
    int i;
    for(i = 0; i < 90; i++){
        logMessage(150, "Angle: %d IR_Value: %d Ping: %.2f\r\n", scanPointArray[i].angle , scanPointArray[i].IR, scanPointArray[i].ping);
    }
}

// #### OBJECT DETECTION LAB 11 - ISAAC's CODE PROBABLY WILL EXPLODE ####

void initalizeObject(int objectId, int startAngle, int endAngle, float distance_cm){
    objectArray[objectId].primary_id = objectId;
    objectArray[objectId].start_angle = startAngle;
    objectArray[objectId].end_angle = endAngle;
    objectArray[objectId].distance_cm = distance_cm;

    objectArray[objectId].middle_angle = (startAngle + endAngle) / 2;
    objectArray[objectId].radial_width = (endAngle - startAngle); //rad
}

//MAKE SURE SCANPOINTARR CONTAINS DATA BEFORE CALLING THIS FUNCTION
int objectDetermination(/*Modifies objectArray; reads from scanPointArray*/){ //Populates global objectArray with objects from the scanPoint Array Data
    int objectCount = 0; //object counter
    int j; //outer loop
    int IR_effectiveRange = 45; //45 CM
    int IR_tolerance = 5;
    //int ping_effectiveRange = 100; //100 CM
    for (j = 0; j < 90; j++) {
        float ping_distance = scanPointArray[j].ping; //reads current ping distance of one point from scanPointArray
        int IR_distance = scanPointArray[j].IR;//reads current IR distance of one point from scanPointArray
        int angle = scanPointArray[j].angle; //reads current angle value of one point from scanPoint Array; could be achieved by reading j too

        //PRIMARILY WILL READ FROM IR for determining object width; and use PING for distance herself

        if (IR_distance < IR_effectiveRange){
            //Need to find out if we have an object by checking the next next value (basically i+2); Assuming that all objects are at least 6 degrees long (ie 3 similar values in a row)
            if (j > 87) continue; // avoid j+2 overflow

            int ir2 = scanPointArray[j+2].IR;

            if (ir2 < IR_distance - IR_tolerance || ir2 > IR_distance + IR_tolerance) continue; //tests false case

            int beginningAngle = angle;
            float distance = ping_distance; //yoink from PING because its more accuarte

            int IR_base_distance = scanPointArray[j].IR; //compares a base distance which moves for every next value, improving object detection (found in prev labs)

            while (j < 90) { //i  like windows
                int next_IR = scanPointArray[j].IR;
                if (abs(next_IR - IR_base_distance) > IR_tolerance) break;
                IR_base_distance = next_IR; // slide the window forward
                j++;
            }

            int endingAngle = scanPointArray[j-1].angle;

            int primitive_radial_width = endingAngle - beginningAngle;
            if(primitive_radial_width < 5){
                continue;
            }

            //Initialize Object
            initalizeObject(objectCount, beginningAngle, endingAngle, distance);
            objectCount++;

        }
    }
    return objectCount;
}

void printObjects(int num){ //number of objects to iterate thru; thinking this param should take input from objectDet
    //logMessage(100, "Object # |  Angle |  Distance(PING) | Radial Width");

    int i;
    for (i = 0; i < num; i++) {
        logMessage(100, "Object: %d | Start Angle: %d; End Angle: %d; Middle Angle: %d |  Distance: %.2f | Width: %d\r\n", objectArray[i].primary_id, objectArray[i].start_angle, objectArray[i].end_angle, objectArray[i].middle_angle, objectArray[i].distance_cm, objectArray[i].radial_width);
    }
}


// #### END OF LAB 11 ####


void clean_data(int data[], int output[])
{
    int temp[COUNT];
    int i = 0;

    for (i = 0; i < COUNT; i++) {
        temp[i] = data[i];
    }

    // remove spikes
    for (i = 1; i < COUNT - 1; i++)
    {
        int diff_prev = abs(temp[i] - temp[i-1]);
        int diff_next = abs(temp[i] - temp[i+1]);

        if (diff_prev > RAW_IR_THRESHOLD && diff_next > RAW_IR_THRESHOLD)
        {
            temp[i] = (temp[i-1] + temp[i+1]) / 2;
        }
    }

    // fix last element spike
    if (abs(temp[COUNT-1] - temp[COUNT-2]) > RAW_IR_THRESHOLD)
    {
        temp[COUNT-1] = temp[COUNT-2];
    }

    // edge preserving smoothing
    for (i = 0; i < COUNT; i++) {
        float sum = temp[i];
        int count = 1;


        // left neighbor
        if (i > 0 && abs(temp[i] - temp[i-1]) < RAW_IR_THRESHOLD) {
            sum += temp[i-1];
            count++;
        }

        // right neighbor
        if (i < COUNT - 1 && abs(temp[i] - temp[i+1]) < RAW_IR_THRESHOLD) {
            sum += temp[i+1];
            count++;
        }

        output[i] = (int)(sum / count);
    }
}

//Fills obstacles[] with obstacle data
int edge_detection(Obstacle obstacles[], int data[], float ping_data[])
{
    int count_up = 0;
    int count_down = 0;
    int ups[10];
    int downs[10];
    int i = 0;

    int current_search_direction = 1; //1==up, 0==down

    int last_distance = data[0];
    for (i = 3; i < COUNT; i++)
    {
        if (current_search_direction && data[i] > last_distance * EDGE_SCALE)
        {
            ups[count_up] = i;
            count_up++;
            current_search_direction = !current_search_direction;
        }

        else if (!current_search_direction && data[i]< last_distance * (2 - EDGE_SCALE))
        {
            downs[count_down] = i;
            count_down++;
            current_search_direction = !current_search_direction;
        }

        last_distance = data[i-2]; //lag behind by 3
    }

    for (i = 0; i < count_down; i++)
    {
        obstacles[i].start_angle = ups[i] * 2; //stores actual angle
        obstacles[i].end_angle = downs[i] * 2; //stores actual angle
        obstacles[i].distance_cm = ping_data[(ups[i] + downs[i]) / (2)];
    }

    return count_down; //number of objects found
}

int find_smallest_object(Obstacle obstacles[], int numObstacles, float ping_data[])
{
    int angle = (obstacles[0].start_angle + obstacles[0].end_angle) / 2;
    int smallest_linear_width = 2 * ping_data[angle/2] * sin((obstacles[0].end_angle - obstacles[0].start_angle) * 3.141592 / (2.0 * 180));
    int smallest_linear_index = 0;
    int i = 0;
    for(i = 1; i < numObstacles; i++)
    {
        int angle = (obstacles[i].start_angle + obstacles[i].end_angle) / 2;
        int linear_width = 2 * ping_data[angle/2] * sin((obstacles[i].end_angle - obstacles[i].start_angle) * 3.141592 / (2.0 * 180));
        if(linear_width < smallest_linear_width)
        {
            smallest_linear_width = linear_width;
            smallest_linear_index = i;
        }

    }
    return smallest_linear_index;
}

void print_obstacles(Obstacle obstacles[], int numObstacles, float ping_data[]) {

    char buffer[32];
    sprintf(buffer, "Objects#  Angle  Distance  Width");
    uart_sendStr(buffer);
    //uart_sendNewLine();
    int i = 0;
    for(i = 0; i < numObstacles; i++)
    {
        int angle = (obstacles[i].start_angle + obstacles[i].end_angle) / 2;
        //Find linear width via trig (Sin of dTheta / 2) = (half of linear width / distance). Solve for linear width. [Draw diagram]
        int linear_width = 2 * ping_data[angle/2] * sin((obstacles[i].end_angle - obstacles[i].start_angle) * 3.141592 / (2.0 * 180)); //convert sin part to radians, also divide by 2
        sprintf(buffer, "%-10d%-7d%-10.2f%-5d", i, angle, ping_data[angle/2], linear_width);
        uart_sendStr(buffer);
        //uart_sendNewLine();
    }
}
