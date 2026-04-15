/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */
void sendString (char *str);

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

//#warning "Possible unimplemented functions"
//#define REPLACEME 0

void sendString (char *str) {
    int i = 0;
    while (i <= strlen(str)) {
        cyBot_sendByte(str[i]);
        i++;
    }
}

int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	cyBot_uart_init();            // Don't forget to initialize the cyBot UART before trying to use it

	// YOUR CODE HERE
	//lcd_printf("please work");
	
	//button_init();
	int input = 0;
	int last_input = 0;

	while(1)
	{
	    lcd_printf("Button: %d", button_getButton());
	    char str[20];
	    input = button_getButton();
	    if(input != last_input){
	        sprintf(str, "Button: %d \r\n", button_getButton());
            sendString(str);
	    }
	    last_input = input;
	    /*
	    sprintf(str, "Button: %d \r\n", button_getButton());
	    while(button_getButton() > 0){
	        sendString(str);
	    }
	    */
	}

}
