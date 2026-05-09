/**
* @file movement.c
* @brief File containing movement functions
*
* @author Michael Farmer
*
* @date 02/04/2026
*/

#include "movement.h"
#include "boundary.h"
#include "log_message.h"
#include "utility.h"
#include "open_interface.h"

/**
* Moves the robot forward by a given distance, using the encoders as feedback. Adjusts encoder mm to real mm using constant.
* @author Michael Farmer, Isaac Cormier, Mila Haynes, Cooper Sanders
* @param self The robot sensor data struct
* @param double_distance_mm The real-world distance to travel
* @date 02/04/2026
*/
double move_forward_mm(oi_t *sensor_data, double distance_mm)
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
            break;
        }
        else if ((*sensor_data).bumpRight)
        {
            status = 6; //right
            break;
        }

        /*BOUNDARY CHECK*/
        int ir_sensor = check_boundary(sensor_data);
        if (ir_sensor != 0)
        {   status = ir_sensor;
            break;
        }
    }

    oi_setWheels(0, 0);

    if(status == 5)
    {
        log_message(100, "BUMP LEFT\r\n");
    }
    else if(status == 6)
    {
        log_message(100, "BUMP RIGHT\r\n");
    }
    else
    {
        if(status == 1){log_message(100, "IR LEFT: %d\r\n", get_left_cliff_sensor(sensor_data));}
        if(status == 2){log_message(100, "IR FRONT LEFT: %d\r\n", get_front_left_cliff_sensor(sensor_data));}
        if(status == 3){log_message(100, "IR FRONT RIGHT: %d\r\n", get_front_right_cliff_sensor(sensor_data));}
        if(status == 4){log_message(100, "IR RIGHT: %d\r\n", get_right_cliff_sensor(sensor_data));}
    }

    move_backward_mm(sensor_data, 50);
    return sum;
}

/*Moves the robot backward by a given distance, using the encoders as feedback. Adjusts encoder mm to real mm using constant.
 * @author Michael Farmer
 * @param self The robot sensor data struct
 * @param double_distance_mm The real-world distance to travel
 * @date 02/04/2026
 */
double move_backward_mm(oi_t *sensor_data, double distance_mm) {

    oi_setWheels(-100,-100);

    double sum = 0;

    double LINEAR_ADJUSTMENT = 1.0;

    while(sum > -distance_mm * LINEAR_ADJUSTMENT) {

        oi_update(sensor_data);

        sum += sensor_data->distance;

    }

    oi_setWheels(0,0); //Stop

    return sum * LINEAR_ADJUSTMENT; //Actual distance traveled

}


/* Turns the robot right by a given angle, using the encoders as feedback.
 * @author Michael Farmer
 * @param self The robot sensor data struct
 * @param degrees The real-world angle to rotate by
 * @date 02/04/26
 */
double turn_right_deg(oi_t *sensor_data, double degrees)
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

/* Turns the robot left by a given angle, using the encoders as feedback.
 * @author Michael Farmer
 * @param self The robot sensor data struct
 * @param degrees The real-world angle to rotate by
 * @date 02/04/26
 */
double turn_left_deg(oi_t *sensor_data, double degrees)
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
