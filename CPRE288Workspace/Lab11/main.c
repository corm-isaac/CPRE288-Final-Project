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
#include "manual.h"
#include "song.h"
#include "imu.h"
#include "i2c.h"

int main (void)
{
    timer_init();
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    lcd_printf("turn the bot on");
    IntMasterEnable();
    adc_init();
    uart_interrupt_init();
    ping_init();
    servo_init();
    imu_init();
    imu_set_ndof_mode(false);
    lcd_printf("init done");



    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    lcd_clear();

    load_songs();

    //uart_sendStr("Press any key to start program: \r\n");
    command_byte = 'n';
    //while(command_flag !=1);
    //servo_calibrate();
    //uart_sendStr("mr forest robot please work\r\n");
    man_drive(sensor_data);

    oi_free(sensor_data);
    return 1;
}
