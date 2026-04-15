/**
 * lab6-interrupt_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"  // For scan sensors
#include "Scan.h"  // For scan sensors
#include "uart-interrupt.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

// Uncomment or add any include directives that you want to use
// #include "open_interface.h"
// #include "movement.h"
// #include "button.h"

// Your code can use the global variables defined in uart-interrupt.c
// They are declared with the extern qualifier in uart-interrupt.h, which makes the variables visible to this file.
//char command_byte;
//char command_flag = 0;
int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_interrupt_init();
	IntMasterEnable();
    //cyBOT_init_Scan(0b);

	// OPTIONAL
	//assign a value to command_byte if you want to know whether that ASCII code is received

	//note that command_byte is global shared variable read by the ISR
	//for example, try using a tab character as a command from PuTTY

	int scanning = 0;
	command_byte = 's';
	lcd_printf("please workkk");

	while(1)
	{

      // YOUR CODE HERE
			//first, try leaving this loop empty and see what happens
			//then add code for your application
			// OPTIONAL
			//test and reset command_flag if your ISR is updating it
			//for example, if the flag is 1, do something, like send a message to PuTTY or LCD, or stop a sensor scan, etc.
			//be sure to reset command_flag so you don't keep responding to an old flag


	    if(command_flag == 1){
	         lcd_clear();

	         lcd_printf("%c", command_byte);
	         command_flag = 0;
	    }


	    /*
	    if(!scanning){
	        char c = uart_receive();
	        if (c == 'g'){
	            uart_sendStr("Stating scan \r\n");
	            scanning = 1;
	        }
	    }

	    if(scanning){
	        //cyBOT_Scan_t scanData;
	        //cyBOT_Scan(90, &scanData);
	        getScan();

	        char msg[50];
	        sprintf(msg, "Distance: %d mm \r\n", scan.sound_dist);
	        uart_sendStr(msg);

	        timer_waitMillis(100);
	    }

	    if (command_flag == 1){

	        uart_sendStr("Command received \r\n");

	        scanning = 0;
	        command_flag = 0;

	        lcd_printf("Stopped");

	    }

        */
	}

}
