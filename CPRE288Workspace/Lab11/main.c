/**
 * @file main.c
 *
 * @brief Main file including component initialization. Currently calls man_drive function to enable manual driving mode.
 *
 * @author Isaac Cormier, Mila Haynes, Michael Farmer, Cooper Sanders
 *
 * @date 4/22/26
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

/*Main method used to initialize components and call man_drive to enable manual driving mode.
 * @author Isaac Cormier, Mila Haynes, Michael Farmer, Cooper Sanders
 * @date 4/22/26
 */
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
    //imu_init();
    //imu_set_ndof_mode(false);
    lcd_printf("init done");

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    lcd_clear();

    load_songs();

    command_byte = 'n';
    man_drive(sensor_data);

    oi_free(sensor_data);
    return 1;
}
