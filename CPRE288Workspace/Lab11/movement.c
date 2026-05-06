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
double move_forward(oi_t *sensor_data, double distance_mm)
{
    double sum = 0;
    int status = 0;


    oi_setWheels(75, 75);

    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        sum += (*sensor_data).distance;


        if ((*sensor_data).bumpLeft)
        {
            status = 5; //left
            //sum -= 75;
            //logMessage(100, "BUMP LEFT\r\n");
            //move_backward(sensor_data, 100);
            break;
        }
        else if ((*sensor_data).bumpRight)
        {
            status = 6; //right
            //sum -= 75;
            //logMessage(100, "BUMP RIGHT\r\n");
            //move_backward(sensor_data, 100);
            break;
        }

        /*BOUNDARY CHECK*/
        int ir_sensor = checkBoundary(sensor_data);
        if (ir_sensor != 0)
        { // returns truthy value
            //logMessage(50, "\r\nBreak -> Sensor Tripped: %d\r\n", sensor);
            status = ir_sensor;
            break;
        }
    }

    oi_setWheels(0, 0);
    if(status == 5)
    {
        logMessage(100, "BUMP LEFT\r\n");
    }
    else if(status == 6)
    {
        logMessage(100, "BUMP RIGHT\r\n");
    }
    else
    {
        if(status == 1){logMessage(100, "IR LEFT: %d\r\n", getLeftCliffSensor(sensor_data));}
        if(status == 2){logMessage(100, "IR FRONT LEFT: %d\r\n", getFrontLeftCliffSensor(sensor_data));}
        if(status == 3){logMessage(100, "IR FRONT RIGHT: %d\r\n", getFrontRightCliffSensor(sensor_data));}
        if(status == 4){logMessage(100, "IR RIGHT: %d\r\n", getRightCliffSensor(sensor_data));}
    }

    move_backward(sensor_data, 50);
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

