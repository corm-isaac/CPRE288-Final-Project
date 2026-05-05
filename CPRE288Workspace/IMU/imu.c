/*
 * imu.c
 *
 *  Created on: Apr 22, 2026
 *      Author: mifarmer
 */
//init

#include "imu.h"
#include "timer.h"


void imu_init() {
    i2c_init();
    timer_init(); //required

    //set_slave_address(0x29); //takes care of GPIO init

    GPIO_PORTB_DIR_R |= 0x40; //make PB6 output
    GPIO_PORTB_AFSEL_R &= ~0x40; // disable alt func for PB6
    GPIO_PORTB_DEN_R |= 0x40; //enable digital io

    GPIO_PORTB_DATA_R &= ~0x40; //set PB6 (RSTN) to 0 (RESET)

    timer_waitMillis(50);

    GPIO_PORTB_DATA_R |= 0X40;

    timer_waitMillis(500);

    ///Now do some writes to IMU
    imu_write_command(0x3E, 0x00); //PWR_MODE --> NORMAL
    imu_write_command(0x3D, 0x00); //OPR_MODE --> CONFIG

    //AXIS Remap to option P0 in the Datasheet
    imu_write_command(0x41, 0x21); //AXIS_REMAP_CONFIG --> See datasheet
    imu_write_command(0x42, 0x4); //AXIS_REMAP_SIGN --> invert X

    imu_write_command(0x3D, 0x0C); //OPR_MODE --> NDOF [Nine Degrees of Freedom]





}

//send a write command to imu
void imu_write_command(char address, char data) {
    char command[2];
    command[0] = address;
    command[1] = data;

    i2c_send_bytes(command, 2);
}

float imu_get_heading_deg() {
    //should be in NDOF
    uint8_t data[2];
    i2c_imu_read_registers(data, 2, 0x1A);

    return ((float)data[0] + (float)(data[1] << 8)) / 16.0f;
}


