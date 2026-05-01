/*
 * boundary.c
 *
 *  Created on: Apr 22, 2026
 *      Author: ikcorm
 */

#include "open_interface.h"
#include "logMessage.h"
//Front Left :2019 -Concrete
//Left Cliff: 1747 - Concrete
//Front Right: 1652 - Concrete
//Right Cliff: 964 - Concrete

//Front Left :2738 -Tape
//Left Cliff: 2733 - Tape
//Front Right: 2700 - Tape
//Right Cliff: 2468 - Tape

//Front Left :16 -Hole
//Left Cliff: 9 - Hole
//Front Right: 4 - Hole
//Right Cliff: 1 - Hole

const uint32_t FRONT_LEFT_CONCRETE = 2019;
const uint32_t LEFT_CONCRETE = 1747;
const uint32_t FRONT_RIGHT_CONCRETE = 1652;
const uint32_t RIGHT_CONCRETE = 964;

const uint32_t FRONT_LEFT_TAPE = 2738;
const uint32_t LEFT_TAPE = 2733;
const uint32_t FRONT_RIGHT_TAPE = 2700;
const uint32_t RIGHT_TAPE = 2468;

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
    oi_update(sensor_data);
    return (*sensor_data).cliffFrontLeftSignal;
}

uint32_t getLeftCliffSensor(oi_t *sensor_data){
    oi_update(sensor_data);
    return (*sensor_data).cliffLeftSignal;
}

uint32_t getFrontRightCliffSensor(oi_t *sensor_data){
    oi_update(sensor_data);
    return (*sensor_data).cliffFrontRightSignal;
}

uint32_t getRightCliffSensor(oi_t *sensor_data){
    oi_update(sensor_data);
    return (*sensor_data).cliffRightSignal;
}

void printValues(oi_t *sensor_data){
    logMessage(60,"Front Left Cliff Sensor: %u\r\n", (getFrontLeftCliffSensor(sensor_data)));
    logMessage(60,"Left Cliff Sensor: %u\r\n", (getLeftCliffSensor(sensor_data)));
    logMessage(60,"Front Right Cliff Sensor: %u\r\n", (getFrontRightCliffSensor(sensor_data)));
    logMessage(60,"Right Cliff Sensor: %u\r\n", (getRightCliffSensor(sensor_data)));
}

int checkBoundary(oi_t *sensor_data){

    int hole_tolerance = 50;;
    int tape_tolerance = -100;

    oi_sensor_val dataValues = {0,0,0,0};

    dataValues.frontLeft = getFrontLeftCliffSensor(sensor_data);
    dataValues.left = getLeftCliffSensor(sensor_data);
    dataValues.frontRight = getFrontRightCliffSensor(sensor_data);
    dataValues.right = getRightCliffSensor(sensor_data);

    //Front Left
    if (dataValues.frontLeft < (FRONT_LEFT_HOLE +  hole_tolerance) || dataValues.frontLeft > (FRONT_LEFT_TAPE +  tape_tolerance)){
        return 1;
    }
    //Left
    else if (dataValues.left < (LEFT_HOLE +  hole_tolerance) || dataValues.left > (LEFT_TAPE +  tape_tolerance)){
        return 2;
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








