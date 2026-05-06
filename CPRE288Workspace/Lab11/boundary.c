/*
 * boundary.c
 *
 *  Created on: Apr 22, 2026
 *      Author: ikcorm
 */

// MAJOR MAJOR MAJOR WARNING THIS FUNCTION DOES NOT EXPLICITLY CALL OI UPDATE (SHOULD BE CALLED IN THE PARENT FUNCTION) BECAUSE OF TIME COMPLEXITY ISSUES IN MOVE FORWARD (MOVEMENT)

#include "open_interface.h"
#include "logMessage.h"
//Front Left :2019 -Concrete - stupid white tile in overflow: 2814
//Left Cliff: 1747 - Concrete -stupid white tile in overflow: 2800
//Front Right: 1652 - Concrete -stupid white tile in overflow: 2778
//Right Cliff: 964 - Concrete - stupid white tile in overflow: 2794

//Front Left :2738 -Tape -using a stupid ruler because of the stupid white tile in overflow: 2950
//Left Cliff: 2733 - Tape - 2967
//Front Right: 2700 - Tape - 2927
//Right Cliff: 2468 - Tape - 2961

//Front Left :16 -Hole
//Left Cliff: 9 - Hole
//Front Right: 4 - Hole
//Right Cliff: 1 - Hole

const uint32_t FRONT_LEFT_CONCRETE = 2814;
const uint32_t LEFT_CONCRETE = 2800;
const uint32_t FRONT_RIGHT_CONCRETE = 2778;
const uint32_t RIGHT_CONCRETE = 2794;

const uint32_t FRONT_LEFT_TAPE = 2950;
const uint32_t LEFT_TAPE = 2967;
const uint32_t FRONT_RIGHT_TAPE = 2927;
const uint32_t RIGHT_TAPE = 2961;

const uint32_t FRONT_LEFT_HOLE = 16;
const uint32_t LEFT_HOLE = 9;
const uint32_t FRONT_RIGHT_HOLE = 4;
const uint32_t RIGHT_HOLE = 1;

typedef struct { //oi sensor values 0-4096
    uint32_t frontLeft;
    uint32_t frontRight;
    uint32_t right;
    uint32_t left;
} oi_sensor_val;


uint32_t getFrontLeftCliffSensor(oi_t *sensor_data){
//    oi_update(sensor_data);
    return (*sensor_data).cliffFrontLeftSignal;
}

uint32_t getLeftCliffSensor(oi_t *sensor_data){
//    oi_update(sensor_data);
    return (*sensor_data).cliffLeftSignal;
}

uint32_t getFrontRightCliffSensor(oi_t *sensor_data){
//    oi_update(sensor_data);
    return (*sensor_data).cliffFrontRightSignal;
}

uint32_t getRightCliffSensor(oi_t *sensor_data){
//    oi_update(sensor_data);
    return (*sensor_data).cliffRightSignal;
}

void printValues(oi_t *sensor_data){
    logMessage(60,"Front Left Cliff Sensor: %u\r\n", (getFrontLeftCliffSensor(sensor_data)));
    logMessage(60,"Left Cliff Sensor: %u\r\n", (getLeftCliffSensor(sensor_data)));
    logMessage(60,"Front Right Cliff Sensor: %u\r\n", (getFrontRightCliffSensor(sensor_data)));
    logMessage(60,"Right Cliff Sensor: %u\r\n", (getRightCliffSensor(sensor_data)));
}

int checkBoundary(oi_t *sensor_data){ //LEFT 1; FRONT LEFT 2 ; FRONT RIGHT 3; RIGHT 4

    int hole_tolerance = 50;
    int tape_tolerance = -80;

    oi_sensor_val dataValues = {0,0,0,0};

    dataValues.frontLeft = getFrontLeftCliffSensor(sensor_data);
    dataValues.left = getLeftCliffSensor(sensor_data);
    dataValues.frontRight = getFrontRightCliffSensor(sensor_data);
    dataValues.right = getRightCliffSensor(sensor_data);

    //Front Left
    if (dataValues.frontLeft < (FRONT_LEFT_HOLE +  hole_tolerance) || dataValues.frontLeft > (FRONT_LEFT_TAPE +  tape_tolerance)){
        return 2;
    }
    //Left
    else if (dataValues.left < (LEFT_HOLE +  hole_tolerance) || dataValues.left > (LEFT_TAPE +  tape_tolerance)){
        return 1;
    }
    //Front Right
    else if(dataValues.frontRight < (FRONT_RIGHT_HOLE +  hole_tolerance) || dataValues.frontRight > (FRONT_RIGHT_TAPE +  tape_tolerance)) {
        return 3;
    }
    //Right
    else if (dataValues.right < (RIGHT_HOLE +  hole_tolerance) || dataValues.right > (RIGHT_TAPE +  tape_tolerance)){
        return 4;
    }

    else {
        return 0;
    }
}








