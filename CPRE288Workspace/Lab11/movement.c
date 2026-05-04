/**
* @file movement.c
* @brief File containing movement functions. Includes move_forward.
*
* @author Michael Farmer
*
* @date 02/04/2026
*/
#include "movement.h"
#include "boundary.h"
#include "logMessage.h"
#include "utility.h"

/**
* Moves the robot forward, using the encoders as feedback. Adjusts encoder mm to real mm using constant.
* @author Michael Farmer
* @param self The robot sensor data struct

* @param double_distance_mm The real-world distance to travel
* @date 02/04/2026
*/
double move_forward(oi_t *sensor_data, double distance_mm) {
    double sum = 0;

        if (distance_mm > 0) {
            oi_setWheels (75,75);

            while (sum < distance_mm){
                oi_update(sensor_data);
                sum += (*sensor_data).distance;

                if ((*sensor_data).bumpLeft){
                    sum -= 75;
                    bumpLeft(sensor_data);
                    oi_setWheels(75, 75);
                } else if ((*sensor_data).bumpRight){
                    sum -= 75;
                    bumpRight(sensor_data);
                    oi_setWheels(75, 75);
                }

                /*BOUNDARY CHECK*/
                int sensor = checkBoundary(sensor_data);
                if(sensor){ //returns truthy value
                    logMessage(50, "\r\nBreak -> Sensor Tripped: %d\r\n", sensor);
                   STOP_BYTE = 1;
                }

                if(STOP_BYTE) {
                      STOP_BYTE = 0;
                      break;
                }
            }
        } else {
            oi_setWheels(-75, -75);

            while (sum > distance_mm) {
                oi_update(sensor_data);
                sum += (*sensor_data).distance;
            }
        }

        oi_setWheels(0,0);
        return sum;
}

double move_backward(oi_t *sensor_data, double distance_mm) {

    oi_setWheels(-100,-100); //Medium speed, accuracy tbd

    double sum = 0;

    double LINEAR_ADJUSTMENT = 1.0; //if mm readings are inaccurate. Will need conceptual revision. Should be in header file

    while(sum > -distance_mm * LINEAR_ADJUSTMENT) {

        oi_update(sensor_data);

        sum += sensor_data->distance;

    }

    oi_setWheels(0,0); //Stop

    return sum * LINEAR_ADJUSTMENT; //Actual distance traveled

}

double turn_right(oi_t *sensor_data, double degrees)
{
    oi_setWheels(-100,100);

    double sum = 0;

    double ROTATIONAL_ADJUSTMENT = 0.9;

    while(sum > -degrees * ROTATIONAL_ADJUSTMENT)
    {
        oi_update(sensor_data);

        sum += sensor_data->angle;
    }

    oi_setWheels(0,0); //Stop

    return sum;
}


double turn_left(oi_t *sensor_data, double degrees)
{

    oi_setWheels(100,-100);

        double sum = 0;

        double ROTATIONAL_ADJUSTMENT = 1;

        while(sum < degrees * ROTATIONAL_ADJUSTMENT)
        {
            oi_update(sensor_data);

            sum += sensor_data->angle;
        }

        oi_setWheels(0,0); //Stop

        return sum;

}


void bumpLeft(oi_t *sensor_data) {

    move_forward(sensor_data, -150);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 250);
    turn_left(sensor_data, 90);
    move_forward(sensor_data, 150);
}

void bumpRight(oi_t *sensor_data) {

    move_forward(sensor_data, -150);
    turn_left(sensor_data, 90);
    move_forward(sensor_data, 250);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 150);
}




void oops(oi_t *sensor_data, int sensor_tripped){

    move_backward(sensor_data, 100); //Move back 15cm

    switch(sensor_tripped){
    case 1: //FRONT LEFT
        turn_right(sensor_data, 45);
        break;
    case 2: //LEFT
        turn_right(sensor_data, 65);
        break;
    case 3: //FRONT RIGHT
        turn_left(sensor_data, 45);
        break;
    case 4: //RIGHT
        turn_left(sensor_data, 65);
        break;
    }
}


float calibrateMovement(oi_t *sensor_data){
    float sum = 0;
    sum = move_forward(sensor_data, 500);
    return sum;
}

