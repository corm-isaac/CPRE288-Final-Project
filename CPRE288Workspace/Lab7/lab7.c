

//void resetGlobals(void);

/**
 * lab6-interrupt_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include <stdarg.h>
#include <stdio.h>

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


void resetGlobals() {
    memset(angle_array, 0, sizeof(angle_array));
    memset(dist_array, 0, sizeof(dist_array));
    memset(ir_array, 0, sizeof(ir_array));
    small_object_index = 0;
}

void inputCyBot(){
    uart_sendStr("Forward=w, Backward=s, left=a, right=b,scan=m, toggle=t\r\n");
}

//deadass copilot wrote this shi don't fail me
void logMessage(size_t bufSize, const char *fmt, ...)
{
    char buffer[bufSize];

    va_list args;
    va_start(args, fmt);

    vsnprintf(buffer, bufSize, fmt, args);

    va_end(args);

    // Output to LCD
    lcd_printf("%s", buffer);

    // Output to UART
    uart_sendStr(buffer);
}


int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_interrupt_init();
	IntMasterEnable();

	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);

	//PART 1
	uart_sendStr("Run Lab 7\r\n");
	command_byte = 'n';
	while(command_flag !=1);

	//char data[100];
    getScan(1);

    int fail_count = 0;

	while(fail_count < 4){

    int IR_range = 1100; //in Voltage (inversely related to distance???)~50cm ~1090 ~~15cm 2267 (anything closer than ~50cm is evaluated)
    int tolerance = 115; //tolerance between objects; 15 is a arbitrary number, needs more testing
	field_object objects[20]; //object aarr
	int numObjects = objectInfo(objects, IR_range, tolerance); //number of objects
	printObjectInfo(objects, numObjects); //printed
	//Object Disp.
    char data[60]; //data buffer
	logMessage(20, "Objects: %d", numObjects);
	//small
	int targetAngle = 0;
	if (numObjects > 0) {
	    targetAngle = smallestObjectDegrees(objects, numObjects);
	    //pointPING(targetAngle);
	}
	else {
	    uart_sendStr("No objects found.\r\n");
	    oi_free(sensor_data);
	    return 0;
	    //return 0;
	}

	//check
	logMessage(40, "Small object dist=%.2f, small_object_index=%d\r\n", objects[small_object_index].distance, small_object_index);
	if(objects[small_object_index].distance < 15){
	    oi_free(sensor_data);
	    return 0;
	}

	//part 4
	uart_sendStr("Part 4\r\n");

	 sprintf(data, "Target Angle=%d", targetAngle);
	 uart_sendStr(data);

	 if(targetAngle < 90){
	     turn_right(sensor_data, 90-targetAngle);
	     }
	     else{
	         turn_left(sensor_data, targetAngle-90);
	     }

	 sprintf(data, "dist:%.2f\r\n",objects[small_object_index].distance);
	 uart_sendStr(data);
	 move_forward(sensor_data, (objects[small_object_index].distance*10) - 150);

	 resetGlobals();
	 getScan(1);
	 fail_count++;
	}
    oi_free(sensor_data);
    return 0;
}

