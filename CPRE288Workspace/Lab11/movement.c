/**
* @file movement.c
* @brief File containing movement functions. Includes move_forward.
*
* @author Michael Farmer
*
* @date 02/04/2026
*/
#include "movement.h"


/**
* Moves the robot forward, using the encoders as feedback. Adjusts encoder mm to real mm using constant.
* @author Michael Farmer
* @param self The robot sensor data struct

* @param double_distance_mm The real-world distance to travel
* @date 02/04/2026
*/
double move_forward(oi_t *sensor_data, double distance_mm, int* bump_return) {

    oi_setWheels(100,100); //Medium speed, accuracy tbd

    double sum = 0;

    double LINEAR_ADJUSTMENT = 1.0; //if mm readings are inaccurate. Will need conceptual revision. Should be in header file

    while(sum < distance_mm * LINEAR_ADJUSTMENT) {

        oi_update(sensor_data);

        sum += sensor_data->distance;

        if(sensor_data->bumpLeft) { //deadzone 25
            //Stop, we have bumped
            *bump_return = -1;
            break;
        } else if(sensor_data->bumpRight) { //deadzone 25
            //Stop, we have bumped
            *bump_return = 1;
            break;
        }

    }

    oi_setWheels(0,0); //Stop

    return sum * LINEAR_ADJUSTMENT; //Actual distance traveled

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


void go_around(oi_t *sensor_data, short direction) {
    move_backward(sensor_data, 150); //Move back 15cm
    int dummy = 0; //Not used
    int* dummy_ptr = &dummy;


    if(direction == 1) { //Right
        turn_right(sensor_data, 90);
        move_forward(sensor_data, 150, dummy_ptr);
        turn_left(sensor_data, 90);
//        move_forward(sensor_data, 400, dummy_ptr);
//       turn_left(sensor_data, 90);
//        move_forward(sensor_data, 150, dummy_ptr);
//       turn_right(sensor_data, 90);
    } else {
        turn_left(sensor_data, 90);
        move_forward(sensor_data, 150, dummy_ptr);
        turn_right(sensor_data, 90);
    }

}
