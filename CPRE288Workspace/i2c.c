/*
 * i2c.c
 *
 *  Created on: Apr 20, 2026
 *      Author: mifarmer
 */
#include "i2c.h"


//Init TM4C registers and I2C basics
void i2c_init() {
    SYSCTL_RCGCI2C_R |= 0x2; //I2C1
    SYSCTL_RCGCGPIO_R |= 0x1; //PA

    while((SYSCTL_PRI2C_R & 0x2) != 0x2){};
    while((SYSCTL_PRGPIO_R & 0x1) != 0x1){};

    GPIO_PORTA_DEN_R |= 0xC0; //PA6, PA7
    GPIO_PORTA_AFSEL_R |= 0xC0; //PA6, PA7
    GPIO_PORTA_ODR_R |= 0x80; //PA7 Open Drain //ETG Excel is wrong lol

    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & ~0xFF000000) | 0x33000000;

    I2C1_MCR_R |= 0x10; //Master mode enable


    //TPR = (System Clock/(2*SCL_LP[[6]] + SCL_HP[[4]])*SCL_CLK))-1
    //IMU supports only up to 300K, so we use 100K for SCL_CLK.
    //I2C1_MTPR_R = (I2C1_MTPR_R & ~0xFF) | 0x7; //Clear HS and TPM bits, set TPM
    I2C1_MTPR_R = 7;
    //
    set_slave_address(0x29); //Per data sheet with ADDR high.


}





void set_slave_address(char addr){

    SYSCTL_RCGCGPIO_R |= 0x2; //PB
    while((SYSCTL_PRGPIO_R & 0x2) != 0x2){};

    GPIO_PORTB_DIR_R |= 0x80; //make PB7 output
    GPIO_PORTB_AFSEL_R &= ~0x80; // disable alt func for PB7
    GPIO_PORTB_DEN_R |= 0x80; //enable digital io

    GPIO_PORTB_DATA_R |= 0x80; //set PB7 (I2C ADDR) to 1;

    addr &= ~0x80; //MSB of char must be 0;
    I2C1_MSA_R = (I2C1_MSA_R & ~0xFF) | addr<<1 + 0x0; //0 for R/S bit

}

//Send single byte, for reading imu registers
void i2c_imu_send_addr(char data){
    I2C1_MSA_R &= ~0x1; // 0 for write

    I2C1_MDR_R = (I2C1_MDR_R & ~0xFF) | data;

    while((I2C1_MCS_R & 0x40) == 0x40){}; //wait until not bus busy

    I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b11; //START RUN and optional STOP

    while((I2C1_MCS_R & 0x01) == 0x01){}; //wait until not busy
    if(I2C1_MCS_R & 0x02) { //ERROR
        I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b100; //STOP
    }

}

void i2c_send_byte(char data) {
    I2C1_MSA_R &= ~0x1; //0

    I2C1_MDR_R = (I2C1_MDR_R & ~0xFF) | data;

    while((I2C1_MCS_R & 0x40) == 0x40){}; //wait until not bus busy

    I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b011; //START RUN and STOP

    while((I2C1_MCS_R & 0x01) == 0x01){}; //wait until not busy
    if(I2C1_MCS_R & 0x02) { //ERROR
        I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b100; //STOP
    }

    I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b100; //STOP
}


void i2c_send_bytes(char* data, uint8_t num_bytes){
    bool error = false;

    //num_bytes 1-indexed
    if(num_bytes > 1) {
        int index = 0;
        I2C1_MSA_R &= ~0x1; //clear 0 bit for transmit
        //I2C1_MSA_R = 0x29<<1 + 0

        I2C1_MDR_R = (I2C1_MDR_R & ~0xFF) | data[index];
        index++;
        while((I2C1_MCS_R & 0x40) == 0x40){}; //wait until not bus busy

        I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b11; //START RUN

        while((I2C1_MCS_R & 0x01) == 0x01){}; //wait until not busy
        if(I2C1_MCS_R & 0x02) { //ERROR
                        I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b100; //STOP
                        error = true;

        }

        while (index < num_bytes - 1 && ~error) {

            I2C1_MDR_R = (I2C1_MDR_R & ~0xFF) | data[index];
            index++;
            while((I2C1_MCS_R & 0x40) == 0x40){}; //wait until not bus busy

            I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b1; //RUN

            if(I2C1_MCS_R & 0x02) { //ERROR
                I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b100; //STOP
                error = true;
                break;
            }

        }
        //Continue final write if no error
        if(~error) {

            I2C1_MDR_R = (I2C1_MDR_R & ~0xFF) | data[index];
            index++;
            //while((I2C1_MCS_R & 0x40) == 0x40){}; //wait until not bus busy

            I2C1_MCS_R = (I2C1_MCS_R & ~0b10111) | 0b101; //STOP RUN
            while((I2C1_MCS_R & 0x01) == 0x01){}; //wait until not busy

            if(I2C1_MCS_R & 0x02) { //ERROR
                error = true;
            }
        }




    } else {
        //Error in num of bytes to send, need to use send_byte
    }


}

uint8_t i2c_imu_read_register(char address) {

    i2c_imu_send_addr(address);
    //Now, we need to do repeated start with non acknowledge 1 byte read

    I2C1_MSA_R |= 0x1; //set 1 to receive

    //I2C1_MCS_R = (I2C1_MCS_R & ~0b11111) | 0b00011; //START RUN, no acknowledge, go to receive mode
    I2C1_MCS_R |= 0b111; //START STOP RUN

    while((I2C1_MCS_R & 0x01) == 0x01){}; //wait until not busy

    uint8_t result;
    if(I2C1_MCS_R & 0x02) { //ERROR
        result = 59;
    } else {
        result = I2C1_MDR_R & 0xFF;
    }

    return result;



}
void i2c_imu_read_registers(uint8_t* return_arr, uint8_t num_bytes, uint8_t start_addr){

    i2c_imu_send_addr(start_addr);


    int data_index = 0;

    I2C1_MSA_R |= 0x1; //set 1 to receive

    I2C1_MCS_R = (I2C1_MCS_R & ~0b11111) | 0b1011; //START RUN, ack

    while((I2C1_MCS_R & 0x01) == 0x01){}; //wait until not busy
    if(I2C1_MCS_R & 0x02) { //ERROR
        return; //early
    }

    while(data_index < num_bytes - 2) {
        return_arr[data_index] = I2C1_MDR_R;
        data_index++;

        I2C1_MCS_R = (I2C1_MCS_R & ~0b11111) | 0b1001; //RUN, ack
        
        while((I2C1_MCS_R & 0x01) == 0x01){}; //wait until not busy
        if(I2C1_MCS_R & 0x02) { //ERROR
            return; //early
        }
    }

    //last byte
    return_arr[data_index] = I2C1_MDR_R;
    data_index++;

    I2C1_MCS_R = (I2C1_MCS_R & ~0b11111) | 0b0101; //RUN, STOP, nack
    
    while((I2C1_MCS_R & 0x01) == 0x01){}; //wait until not busy
    if(I2C1_MCS_R & 0x02) { //ERROR
        return_arr[data_index] = 0x69;
        return; //early
    }

    return_arr[data_index] = I2C1_MDR_R;

    //Now return_arr should be updated.

}



