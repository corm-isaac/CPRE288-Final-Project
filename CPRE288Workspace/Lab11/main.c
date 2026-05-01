/**
 * lab5_template.c
 *
 * Template file for CprE 288 Lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 * @author Diane Rover, updated 2/25/2021, 2/17/2022
 */

#include "Timer.h"
#include "lcd.h"

#include "uart-interrupt.h"
#include "utility.h"
#include "open_interface.h"
#include "movement.h"
#include "adc.h"
#include "ping.h"
#include "button.h"
#include "servo.h"
#include "logMessage.h"
#include "boundary.h"

int main (void) {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    timer_init();
    IntMasterEnable();
    adc_init();
    uart_interrupt_init();
    ping_init();

    uart_sendStr("Run test\r\n");
    command_byte = 'n';
    char exit_char = 0;
    while(!exit_char){
        while(command_flag !=1);
        switch(command_byte){
        case 'w': //forward
            move_forward(sensor_data, 100); //10cm
            logMessage(15, "Forward 10\r\n");
            command_byte = 'n';
            break;
        case 'a': // keft
            turn_left(sensor_data, 10);
            logMessage(15, "Left 10\r\n");
            command_byte = 'n';

            break;
        case 's': //back
            move_backward(sensor_data, 100); //10cm
            logMessage(15, "Back 10\r\n");
            command_byte = 'n';

            break;
        case 'd': //right
            turn_right(sensor_data, 10);
            logMessage(15, "Right 10\r\n");

            command_byte = 'n';
            break;
        case 'e': //exit
            exit_char = 1;
            command_byte = 'n';
            break;
        case 'n':
            logMessage(15, "Waiting...\r\n");
            while(command_byte == 'n');
            break;
        default:
            logMessage(40, "Button not pressed correctly: %c\r\n", command_byte);
            command_byte = 'n';
        }

    }
    oi_free(sensor_data);
}
