/*
 * manual.c
 *
 *  Created on: May 1, 2026
 *      Author: ikcorm
 */

// ASSUMMING INTERRUPTS ARE ENABLED IF NOT THE CODE WILL BLOW UP


#include "Timer.h"
#include "lcd.h"
#include "manual.h"
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
//#include "imu.h"

//ScanPoint scanPointArray[90];



void man_drive(oi_t *sensor_data){
    /*
    uart_sendStr("Don't Moneyshift the cybot\r\n");

    uart_sendStr("Traverse Forward - 'w' \r\n");
    uart_sendStr("Traverse Backward - 's' \r\n");
    uart_sendStr("Traverse Left - 'a' \r\n");
    uart_sendStr("Traverse Right - 'd' \r\n");
    uart_sendStr("180 Scan - 'i' \r\n");
    uart_sendStr("Exit Loop (Hopefully calls oi_free and turns mr robot off - 'e' \r\n"); //call this when done with bot
    uart_sendStr("Wait - 'n' \r\n");

*/


    command_byte = 'n';
    char exit_char = 0;
    while(!exit_char)
    {
        while(command_flag !=1);

        switch(command_byte)
        {
            case 'w': //forward
                move_forward(sensor_data, 100); //10cm
                //logMessage(15, "Forward 10\r\n");
                command_byte = 'n';
                break;

            case 'a': // left
                turn_left(sensor_data, 10);
                //logMessage(15, "Left 10\r\n");
                command_byte = 'n';
                break;

            case 's': //back
                move_backward(sensor_data, 100); //10cm
                //logMessage(15, "Back 10\r\n");
                command_byte = 'n';
                break;

            case 'd': //right
                turn_right(sensor_data, 10);
                //logMessage(15, "Right 10\r\n");
                command_byte = 'n';
                break;

            case 'i':{
                //logMessage(20, "\r\n180 Scan\r\n");
                int i;
                for(i = 0; i < 180; i += 2)
                {
                    scanPointArray[i/2].angle = i;
                    scanPointArray[i/2].IR = IRScan(i);
                    scanPointArray[i/2].ping = PingScan(i);
                }
                //lcd_printf("%d", i);
                logScan(); //prints ScanPointArray
                //Finds objects in theory
                printObjects(objectDetermination());
                command_byte = 'n';
                break;
            }
            case 'p': //update position and angle
                logMessage(60, "\r\nRight 10\r\n");
                command_byte = 'n';
                break;

            case 'v': //vacuum
                suck(2000);
                command_byte = 'n';
                break;

            case 'm': //music
                play_song(1);
                command_byte = 'n';
                break;

            case 'e': //exit
                exit_char = 1;
                //logMessage(80, "\r\nHypothetically I turn the cybot off\r\n");
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

        //lcd_printf("Heading: %f", imu_get_heading_deg());
        //timer_waitMillis(250);
    }
}



