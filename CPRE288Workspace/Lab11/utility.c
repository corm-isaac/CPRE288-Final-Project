/*
 * utility.c
 *
 *  Created on: Mar 24, 2026
 *      Author: cooperrs
 */

#include "utility.h"

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
    uart_sendNewLine();
    int i = 0;
    for(i = 0; i < numObstacles; i++)
    {
        int angle = (obstacles[i].start_angle + obstacles[i].end_angle) / 2;
        //Find linear width via trig (Sin of dTheta / 2) = (half of linear width / distance). Solve for linear width. [Draw diagram]
        int linear_width = 2 * ping_data[angle/2] * sin((obstacles[i].end_angle - obstacles[i].start_angle) * 3.141592 / (2.0 * 180)); //convert sin part to radians, also divide by 2
        sprintf(buffer, "%-10d%-7d%-10.2f%-5d", i, angle, ping_data[angle/2], linear_width);
        uart_sendStr(buffer);
        uart_sendNewLine();
    }
}
