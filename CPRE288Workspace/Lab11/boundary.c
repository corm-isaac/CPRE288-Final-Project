/*
 * @file boundary.c
 * @brief Detects floor obstacles including tape boundary and holes.
 * @author Isaac Cormier, Mila Haynes
 * @date 04/22/26
 */

#include "open_interface.h"
#include "log_message.h"

//Average values
//Front Left: 2019 - Concrete - 2814 (overflow)
//Left Cliff: 1747 - Concrete - 2800 (overflow)
//Front Right: 1652 - Concrete - 2778 (overflow)
//Right Cliff: 964 - Concrete - 2794 (overflow)

//Front Left: 2738 - Tape - 2950 (overflow)
//Left Cliff: 2733 - Tape - 2967 (overflow)
//Front Right: 2700 - Tape - 2927 (overflow)
//Right Cliff: 2468 - Tape - 2961 (overflow)

//Front Left: 16 - Hole
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
    uint32_t front_left;
    uint32_t front_right;
    uint32_t right;
    uint32_t left;
} oi_sensor_val;

/*Returns Front Left Cliff Sensor value
 * @author Isaac Cormier, Mila Haynes
 * @param self The robot sensor data struct
 * @date 04/22/26
 */
uint32_t get_front_left_cliff_sensor(oi_t *sensor_data){
//    oi_update(sensor_data);
    return (*sensor_data).cliffFrontLeftSignal;
}

/*Returns Left Cliff Sensor value
 * @author Isaac Cormier, Mila Haynes
 * @param self The robot sensor data struct
 * @date 04/22/26
 */
uint32_t get_left_cliff_sensor(oi_t *sensor_data){
//    oi_update(sensor_data);
    return (*sensor_data).cliffLeftSignal;
}

/*Returns Front Right Cliff Sensor value
 * @author Isaac Cormier, Mila Haynes
 * @param self The robot sensor data struct
 * @date 04/22/26
 */
uint32_t get_front_right_cliff_sensor(oi_t *sensor_data){
//    oi_update(sensor_data);
    return (*sensor_data).cliffFrontRightSignal;
}

/*Returns Right Cliff Sensor value
 * @author Isaac Cormier, Mila Haynes
 * @param self The robot sensor data struct
 * @date 04/22/26
 */
uint32_t get_right_cliff_sensor(oi_t *sensor_data){
//    oi_update(sensor_data);
    return (*sensor_data).cliffRightSignal;
}

/*Prints values of cliff sensor functions
 * @author Isaac Cormier, Mila Haynes
 * @param self The robot sensor data struct
 * @date 04/22/26
 */
void print_values(oi_t *sensor_data){
    logMessage(60,"Front Left Cliff Sensor: %u\r\n", (get_front_left_cliff_sensor(sensor_data)));
    logMessage(60,"Left Cliff Sensor: %u\r\n", (get_left_cliff_sensor(sensor_data)));
    logMessage(60,"Front Right Cliff Sensor: %u\r\n", (get_front_right_cliff_sensor(sensor_data)));
    logMessage(60,"Right Cliff Sensor: %u\r\n", (get_right_cliff_sensor(sensor_data)));
}

/*Checks if CyBot has crossed boundary
 * @author Isaac Cormier, Mila Haynes
 * @param self The robot sensor data struct
 * @date 04/22/26
 */
int check_boundary(oi_t *sensor_data){
    //LEFT 1; FRONT LEFT 2 ; FRONT RIGHT 3; RIGHT 4

    int hole_tolerance = 50;
    int tape_tolerance = -80;

    oi_sensor_val data_values = {0,0,0,0};

    data_values.front_left = get_front_left_cliff_sensor(sensor_data);
    data_values.left = get_left_cliff_sensor(sensor_data);
    data_values.front_right = get_front_right_cliff_sensor(sensor_data);
    data_values.right = get_right_cliff_sensor(sensor_data);

    //Front Left
    if (data_values.front_left < (FRONT_LEFT_HOLE +  hole_tolerance) || data_values.front_left > (FRONT_LEFT_TAPE +  tape_tolerance)){
        return 2;
    }
    //Left
    else if (data_values.left < (LEFT_HOLE +  hole_tolerance) || data_values.left > (LEFT_TAPE +  tape_tolerance)){
        return 1;
    }
    //Front Right
    else if(data_values.front_right < (FRONT_RIGHT_HOLE +  hole_tolerance) || data_values.front_right > (FRONT_RIGHT_TAPE +  tape_tolerance)) {
        return 3;
    }
    //Right
    else if (data_values.right < (RIGHT_HOLE +  hole_tolerance) || data_values.right > (RIGHT_TAPE +  tape_tolerance)){
        return 4;
    }

    else {
        return 0;
    }
}








