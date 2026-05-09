/*
 * @file manual.c
 * @brief File containing manual movement control
 *
 * @author Isaac Cormier, Mila Haynes
 *
 * @date 05/01/26
 */

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
#include "log_message.h"
#include "boundary.h"
#include "song.h"
#include "imu.h"
#include "i2c.h"

/*
 * Enables and controls manual driving mode.
 * @author Isaac Cormier, Mila Haynes
 * @param self The robot sensor data struct
 * @date 05/01/26
 */
void man_drive(oi_t *sensor_data){

    command_byte = 'n';
    char exit_char = 0;
    while(!exit_char)
    {
        while(command_flag !=1){};

        if(command_byte != 'n')
        {
            lcd_printf("%c  %x", command_byte, command_byte);
        }

        switch(command_byte)
        {
            case 'w': //forward
                command_flag = 0;
                command_byte = 'n';
                move_forward_mm(sensor_data, 100); //10cm

                break;

            case 'a': // left
                command_flag = 0;
                command_byte = 'n';
                turn_left_deg(sensor_data, 10);

                break;

            case 's': //back
                command_flag = 0;
                command_byte = 'n';
                move_backward_mm(sensor_data, 100); //10cm

                break;

            case 'd': //right
                command_flag = 0;
                command_byte = 'n';
                turn_right_deg(sensor_data, 10);

                break;

            case 'i':{ //scan
                command_flag = 0;
                command_byte = 'n';
                int i;
                for(i = 0; i < 180; i += 2)
                {
                    scan_point_array[i/2].angle = i;
                    scan_point_array[i/2].IR = IR_scan(i);
                    scan_point_array[i/2].ping = ping_scan(i);
                }
                log_scan();
                print_objects(object_determination());

                break;
            }
            case 'p': //update position and angle
                command_flag = 0;
                command_byte = 'n';
                log_message(60, "\r\nRight 10\r\n");

                break;

            case 'v': //vacuum
                command_flag = 0;
                command_byte = 'n';
                suck(2000);

                break;

            case 'm': //music
                command_flag = 0;
                command_byte = 'n';
                play_song(1);

                break;

            case 'u': //IMU get heading
                //log_message(30, "\r\nANGLE:%.2f\r\n", imu_get_heading_deg()); //send imu angle over
                command_byte = 'n';
                break;


            case 'e': //exit
                command_flag = 0;
                command_byte = 'n';
                exit_char = 1;

                break;

            case 'n':
                while(command_byte == 'n');

                break;

            default:
                command_flag = 0;

                log_message(40, "\r\nButton not pressed correctly: %c    %x\r\n", command_byte, command_byte);
                command_byte = 'n';

        }

    }
}



