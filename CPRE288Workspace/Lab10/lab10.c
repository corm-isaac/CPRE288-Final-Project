/**
 * @file lab10.c
 * @author
 * Template file for CprE 288 Lab 10
 */

#include "Timer.h"
#include "lcd.h"
#include "ping.h"
#include "Scan.h"  // For scan sensors
#include "servo.h"
#include "logMessage.h"
//#include "cyBot_Scan.h"


#include "uart-interrupt.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"


int main(void) {

    logMessage_init();

	ping_init();

	uart_interrupt_init();

	// YOUR CODE HERE

	logMessage(40, "Running Lab 10: servo_init\r\n");

	servo_init();
	/*
	servo_move(90);

	servo_move(30);

	servo_move(150);

	servo_move(90);
	*/
}
