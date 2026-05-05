/*
 * path.c
 *
 *  Created on: May 3, 2026
 *      Author: ikcorm
 */



//assuming that everything else works
//IMU reads correct values and we have a map?

//Where do we want to go?
// home

// 4 pillars

//Find Corner - To build map? (possibly obsolete)

//
//Test field; relative to the test field; tested by isaac
#define TEST_FIELD_NORTH 207.0   // field north is 207° clockwise from true north
#define TEST_FIELD_EAST  297.0
#define TEST_FIELD_SOUTH 27.0
#define TEST_FIELD_WEST  117.0

//True consts idk if we use but looks cool
#define TRUE_NORTH 0.0
#define TRUE_EAST 90.0
#define TRUE_SOUTH 180.0
#define TRUE_WEST 270.0

#include "manual.h"
#include "open_interface.h"
#include "uart-interrupt.h"
#include "logMessage.h"
#include "utility.h"
#include "servo.h"
volatile enum{NORTH, EAST, SOUTH, WEST, NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST, UNKNOWN} cybot_direction = UNKNOWN; // State of orientation/direction; possibly important

void auto_scan(oi_t *sensor){
    //Scan (not in a method because that would get weird with commits)
    uart_sendStr("\r\n Finding a Corner...\r\n");
    logMessage(20, "\r\n180 Scan\r\n");
    int i;
    for(i = 0; i < 180; i += 2){
        scanPointArray[i/2].angle = i;
        scanPointArray[i/2].IR = IRScan(i);
        scanPointArray[i/2].ping = PingScan(i);
    }
    logScan(); //prints ScanPointArray
    //Finds objects in theory
    printObjects(objectDetermination());
    command_byte = 'n';
    cleanGlobals(); //resets objectArray
    //End Scan
}

void auto_path(oi_t *sensor){
    //stage 1: find a corner
    //stage 2: scan for objects
}

void auto_corner(oi_t *sensor){
    //find a corner then log the data to the IMU

    //scan first; find the path with the least amnt of objects then proceed that way until you find tape; then turn right or left to find a corner;
    //after finding a corner, based off the heading we can determine the boundaries of the test field
    while(1){
        auto_scan(sensor);
        //Object Array should contain data
        //Assuming we can take heading from IMU if not then this is kinda a lot harder
        //cybot heading (taken from IMU)
        float cybot_heading = 0.0; //not implemented yet
        auto_orient(cybot_heading);


    }

}

//Does some calculation to determine where the cybot is pointing in relation to the test field
void auto_orient(float cybot_heading){
    //cybot_heading is taken from IMU

    if(cybot_heading > 207.0 && cybot_heading < 297.0){ //NORTHEAST
        cybot_direction = NORTH_WEST;
    }
    else if(cybot_heading > 297.0 || cybot_heading < 27.0){ //SOUTH EAST
        cybot_direction = SOUTH_EAST;
    }
    else if(cybot_heading > 27.0 && cybot_heading < 117.0){ //SOUTH WEST
        cybot_direction = SOUTH_WEST;
    }
    else { //NORTH WEST
        cybot_direction = NORTH_WEST;
    }
}

//Finds the least amnt of objects for Stage 1 (Finding a corner)
void auto_data_least_objects(){
    //Assuming scan has operated

}



void auto_drive(oi_t *sensor){

    uart_sendStr("Autonomous Driving \r\n");
    uart_sendStr("Toggle to Manual - 't'\r\n");

        command_byte = 'n';
        char exit_char = 0;
        while(!exit_char){
            while(command_flag !=1);
            switch(command_byte){
            case 't': //back to stick
                man_drive(sensor);
                command_byte = 'n';
                break;
            case 'a': //a- carry out next command in algorithim
                auto_path(sensor);
                break;
            case 'e': //exit
                exit_char = 1;
                logMessage(80, "\r\nHypothetically I turn the cybot off\r\n");
                command_byte = 'n';
                break;
            case 'n':
                logMessage(15, "\r\nWaiting...\r\n");
                while(command_byte == 'n');
                break;
            default:
                logMessage(40, "\r\nButton not pressed correctly: %c\r\n", command_byte);
                command_byte = 'n';
            }

        }
}
