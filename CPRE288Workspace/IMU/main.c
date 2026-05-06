/**
 * lab5_template.c
 *
 * Template file for CprE 288 Lab 5
 *
 * @author Michael Farmer
 * @date 04/14/2026
 *
 * @author Phillip Jones, updated 6/4/2019
 * @author Diane Rover, updated 2/25/2021, 2/17/2022
 */

#include "Timer.h"
#include "lcd.h"

#include "uart-interrupt.h"
#include <stdbool.h>
#include "open_interface.h"
#include "adc.h"
#include "oi_song.h"
#include "i2c.h"
#include "imu.h"

volatile char command_byte;
volatile int command_flag;


int main(void)
{
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    lcd_printf("not working");
    uart_interrupt_init();


    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    lcd_clear();

    load_songs();

    //oi_play_song(1);

    /*
    uart_sendChar4(138);
    uart_sendChar4(2); //vac on
    timer_waitMillis(1500);
    uart_sendChar4(138);
    uart_sendChar4(0); //vac on
    */

    imu_init();
    timer_waitMillis(250);
    lcd_printf("3 to set init heading"); //would be some sort of waiting
    timer_waitMillis(3000);

    //imu_set_compass_mode(); //Does calibration loop also and gets initial heading
    timer_waitMillis(250);
    imu_set_ndof_mode();

    while(1) {

        //lcd_printf("%x", i2c_imu_read_register(0x3D));
        lcd_printf("Heading: %f\nMode: %X\nCALIB: %X", imu_get_heading_deg(), i2c_imu_read_register(0x3D), i2c_imu_read_register(0x35));

        timer_waitMillis(250);
    }



}
