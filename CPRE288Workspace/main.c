/*
 * main.c
 *
 *  Created on: Feb 4, 2026
 *      Author: milajune
 */

  //cybot 16
//prototype

void sendString (char *str);
void getScan();


#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
#include "cyBot_uart.h"
#include <string.h>
#include "cyBot_Scan.h"

//global vars

int angle_array[90];
float dist_array[90];

/*
 * cyBot testing
 * 2/16:
 * 232750 right(0) val
 * 1198750 left(180) val
 */

void sendString (char *str){
    int i =0;
    while(i <= strlen(str)){
        cyBot_sendByte(str[i]);
        i++;
    }
}

void getScan(){
    cyBOT_init_Scan(0b0111);
    cyBOT_Scan_t scan;

    right_calibration_value = 253750; //as of 2/17 -cyBot 27
    left_calibration_value = 1240750;

    sendString("Beginning Scan\n");
    char data[50];
    int range;
    range = 180;
    int angle;
    for (angle = 0; angle <= range; angle+=2){
        cyBOT_Scan(angle, &scan);
        sprintf(data, "Angle: %d\t Distance: %.2f \r\n", angle, scan.sound_dist);

        //global var update
        //angle_array
        //dist_array
        angle_array[angle/2] = angle;
        dist_array[angle/2] = scan.sound_dist;


        sendString(data);
    }
    free(&scan);

}


int main (void) {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    timer_init();
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    cyBot_uart_init();

    //lcd_printf("%c", (char)cyBot_getByte()); //ascii char of what was typed in PuTTY
    //char str[] = "fml";
    //int size = strlen(str);
    char str[] = "fml";
    sendString(str);

    //cyBOT_SERVO_cal();



    //cyBOT_Scan_t scan = calloc(1, sizeof(cyBOT_Scan_t));
    cyBot_getByte(); //blocking function
    getScan();

    int threshold_dist = 30; //distance to begin to register objects

    //parse through dist_array, look for values <= 30 or greater than 1



    oi_free(sensor_data);


}









