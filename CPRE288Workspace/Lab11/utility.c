/*
 * @file utility.c
 * @brief random code that is too cool to fit elsewhere. In this instance it's mostly the object detections
 * logic. and random code
 *
 *  @author Cooper Sanders, Isaac Cormier
 *
 *  @date 03/24/2026
 */

#include "utility.h"
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
// GLOBAL VARIABLES - Very pretty unsafe but i dont care
char STOP_BYTE = 0;                // Designed to be manual override to stop the bot. not yet implemented 5/2
Scan_point g_scan_point_array[90]; // An array of ScanPoint objects - ScanPoint objects store the angle, IR and PING value at each angle increment
Obstacle g_object_array[30];       // An array of objects that we have

/*
 *  @brief prints out scan from scanPointArray
 * @author Isaac Cormier, Cooper Sanders
 * @param (takes global scanPointArray)
 * @date 04/26/26
 */
void log_scan()
{
    // logMessage(20, "SCAN_START:\r\n");
    int i;
    for (i = 0; i < 90; i++)
    {
        // logMessage(150, "Angle: %d IR_Value: %d Ping: %.2f\r\n", scanPointArray[i].angle , scanPointArray[i].IR, scanPointArray[i].ping);
        logMessage(150, "SCAN:%d %d %.2f\r\n",
                   g_scan_point_array[i].angle,
                   g_scan_point_array[i].IR,
                   g_scan_point_array[i].ping);
    }
    // logMessage(20, "SCAN_END:\r\n");
}

// #### OBJECT DETECTION LAB 11 - ISAAC's CODE PROBABLY WILL EXPLODE ####
/*
 *  @brief init object
 * @author Mila Haynes, Isaac Cormier,
 * @param (takes global objectArray)
 * @date 04/26/26
 */

void initalize_object(int object_id, int start_angle, int end_angle, float distance_cm)
{
    object_array[object_id].primary_id = object_id;
    object_array[object_id].start_angle = start_angle;
    object_array[object_id].end_angle = end_angle;
    object_array[object_id].middle_angle = (start_angle + end_angle) / 2;
    object_array[object_id].distance_cm = distance_cm;
    object_array[object_id].radial_width = (end_angle - start_angle); // rad
}

/*
 *  @brief determines objects from scanPointArray. most of the code here was taken from our lab7 object determination which was mostly
 * written  by Mila, modified merely to use both IR and Ping seamlessly.
 * @author Mila Haynes, Isaac Cormier
 * @param (takes global objectArray)
 * @date 04/26/26
 */

// MAKE SURE SCANPOINTARR CONTAINS DATA BEFORE CALLING THIS FUNCTION
int object_determination(/*Modifies objectArray; reads from scanPointArray*/)
{                                // Populates global objectArray with objects from the scanPoint Array Data
    int object_count = 0;        // object counter
    int j;                       // outer loop
    int IR_effective_range = 45; // 45 CM
    int IR_tolerance = 5;
    // int ping_effectiveRange = 100; //100 CM
    for (j = 0; j < 90; j++)
    {
        float ping_distance = g_scan_point_array[j].ping; // reads current ping distance of one point from scanPointArray
        int IR_distance = g_scan_point_array[j].IR;       // reads current IR distance of one point from scanPointArray
        int angle = g_scan_point_array[j].angle;          // reads current angle value of one point from scanPoint Array; could be achieved by reading j too

        // PRIMARILY WILL READ FROM IR for determining object width; and use PING for distance herself

        if (IR_distance < IR_effective_range)
        {
            // Need to find out if we have an object by checking the next next value (basically i+2); Assuming that all objects are at least 6 degrees long (ie 3 similar values in a row)
            if (j > 87)
                continue; // avoid j+2 overflow

            int ir2 = g_scan_point_array[j + 2].IR;

            if (ir2 < IR_distance - IR_tolerance || ir2 > IR_distance + IR_tolerance)
                continue; // tests false case

            int beginning_angle = angle;
            float distance = ping_distance; // yoink from PING because its more accuarte

            int IR_base_distance = g_scan_point_array[j].IR; // compares a base distance which moves for every next value, improving object detection (found in prev labs)

            while (j < 90)
            { // i  like windows
                int next_IR = g_scan_point_array[j].IR;
                if (abs(next_IR - IR_base_distance) > IR_tolerance)
                    break;
                IR_base_distance = next_IR; // slide the window forward
                j++;
            }

            int ending_angle = g_scan_point_array[j - 1].angle;

            int primitive_radial_width = ending_angle - beginning_angle;
            if (primitive_radial_width < 5)
            {
                continue;
            }

            // Initialize Object
            initalize_object(object_count, beginning_angle, ending_angle, distance);
            object_count++;
        }
    }
    return object_count;
}

/*
 *  @brief prints the object outs
 * @author Mila Haynes, Isaac Cormier, Cooper Sanders
 * @param (takes global scanPointArray)
 * @date 04/26/26
 */

void print_objects(int num)
{ // number of objects to iterate thru; thinking this param should take input from objectDet
    // logMessage(100, "Object # |  Angle |  Distance(PING) | Radial Width");

    int i;
    for (i = 0; i < num; i++)
    {
        // logMessage(100, "Object: %d | Start Angle: %d; End Angle: %d; Middle Angle: %d |  Distance: %.2f | Width: %d\r\n", objectArray[i].primary_id, objectArray[i].start_angle, objectArray[i].end_angle, objectArray[i].middle_angle, objectArray[i].distance_cm, objectArray[i].radial_width);
        logMessage(100, "OBJECT:%d %d %d %d %.2f %d\r\n",
                   g_object_array[i].primary_id,
                   g_object_array[i].start_angle,
                   g_object_array[i].end_angle,
                   g_object_array[i].middle_angle,
                   g_object_array[i].distance_cm,
                   g_object_array[i].radial_width);
    }
}

// #### END OF LAB 11 ####
/*
 *  @brief mr. wishy washy
 * @author Cooper Sanders
 * @param none
 * @date 04/26/26
 */

void clean_data(int data[], int output[])
{
    int temp[COUNT];
    int i = 0;

    for (i = 0; i < COUNT; i++)
    {
        temp[i] = data[i];
    }

    // remove spikes
    for (i = 1; i < COUNT - 1; i++)
    {
        int diff_prev = abs(temp[i] - temp[i - 1]);
        int diff_next = abs(temp[i] - temp[i + 1]);

        if (diff_prev > RAW_IR_THRESHOLD && diff_next > RAW_IR_THRESHOLD)
        {
            temp[i] = (temp[i - 1] + temp[i + 1]) / 2;
        }
    }

    // fix last element spike
    if (abs(temp[COUNT - 1] - temp[COUNT - 2]) > RAW_IR_THRESHOLD)
    {
        temp[COUNT - 1] = temp[COUNT - 2];
    }

    // edge preserving smoothing
    for (i = 0; i < COUNT; i++)
    {
        float sum = temp[i];
        int count = 1;

        // left neighbor
        if (i > 0 && abs(temp[i] - temp[i - 1]) < RAW_IR_THRESHOLD)
        {
            sum += temp[i - 1];
            count++;
        }

        // right neighbor
        if (i < COUNT - 1 && abs(temp[i] - temp[i + 1]) < RAW_IR_THRESHOLD)
        {
            sum += temp[i + 1];
            count++;
        }

        output[i] = (int)(sum / count);
    }
}

void suck(int time)
{
    oi_uartSendChar(138);
    oi_uartSendChar(2);
    timer_waitMillis(time);
    oi_uartSendChar(138);
    oi_uartSendChar(0);
}
