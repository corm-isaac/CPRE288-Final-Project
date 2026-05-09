/*
 * map.c
 *
 *  Created on: May 3, 2026
 *      Author: ikcorm
 */

//assuming we take IMU coordinate values somehow
#include "utility.h"
#include "map.h"
#include <math.h>

#define DEG_TO_RAD (M_PI / 180.0)

Point determineObjectCoordinates(int objectId) { //determines the coordinates of the objects from the cybots imu and ping/IR sensor
    Obstacle object = objectArray[objectId];
    //whats an IMU
    float IMU_x = 0;
    float IMU_y = 0;
    float IMU_heading = 0;

    // radians
    double total_angle = (IMU_heading + object.middle_angle) * DEG_TO_RAD;

    Point obj;
    obj.x = IMU_x + object.distance_cm * sin(total_angle);
    obj.y = IMU_y + object.distance_cm * cos(total_angle);

    return obj;
}
