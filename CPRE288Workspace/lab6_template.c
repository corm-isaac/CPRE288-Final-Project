/**
 * lab6_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include "Timer.h"
#include "lcd.h"
#include "uart.h"
#include "Scan.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

// Uncomment or add any include directives that are needed
// #include "open_interface.h"
// #include "movement.h"
// #include "button.h"




int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_init();
    cyBOT_init_Scan(0b0111);

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

	// YOUR CODE HERE
    uart_sendStr("Am I working?");
    char input = uart_receive_nonblocking();
    while(input != 'g'){
        input = uart_receive_nonblocking();
    }
    while(1){
        input = uart_receive_nonblocking();
        if(input == 's'){
            break;
        }
        getScan();
    }

}
