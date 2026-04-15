/**
 * lab8-interrupt_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

//y = -2.3789x + 1703; y = IR quant val; x = actual val
// x = ((y-1703)/-2.3789) actual equation

//equation 2
//y = 9797.3x^-0.564
//x = sqrt(-0.564) ((1/9797) y)

#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"  // For scan sensors
#include "Scan.h"  // For scan sensors
#include "uart-interrupt.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

#include "adc.h"
#include <math.h>

// Uncomment or add any include directives that you want to use
// #include "open_interface.h"
// #include "movement.h"
// #include "button.h"

// Your code can use the global variables defined in uart-interrupt.c
// They are declared with the extern qualifier in uart-interrupt.h, which makes the variables visible to this file.
//char command_byte;
//char command_flag = 0;

float convertADCtoCM(uint16_t adcValue){ //cybot 8
    float fugly = (float)adcValue / 9439.6f;
    return powf(fugly, -1.0f / 0.572); //.577
    //double distance;
    //distance = (665394.7619 * pow(adcValue, -1.6968));
    //return distance;
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
	adc_init();

	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);

	//PART 1

	uart_sendStr("part 1\r\n");

	command_byte = 'n';
	while(command_flag !=1);

	cyBOT_init_Scan(0b0111);

	cyBOT_Scan_t scan;

    //cyBOT_SERVO_cal();

	right_calibration_value = 290500;
	left_calibration_value = 129350;
	/*
	uint16_t data[10];
	int angles[5]= {0,45,90,135,180};
	char message[100];
	int i;
	for(i = 0; i < 5; i++){
	    cyBOT_Scan(angles[i], &scan);
	    data[i] = adc_read();
	    sprintf(message, "Angle: %d, Dist: %d\r\n", angles[i], data[i]);
	    uart_sendStr(message);
	 }
    */
	/*
	int angle_array[18];
	float ping_array[18];
	uint16_t dist_array[18];
    float cm_array[18];
    */
    uart_sendStr("Beginning Sample!\r\n");
    /*
	int i;
	int loop_counter = 0;
	for(i = 90; i < 91; i+=10){
	    cm_array[loop_counter] = 0;
	    cyBOT_Scan(i, &scan); //18 samples
	    dist_array[loop_counter] = adc_read();
	    ping_array[loop_counter] = scan.sound_dist;
	    angle_array[loop_counter] = i;
	    cm_array[loop_counter] = convertADCtoCM(dist_array[loop_counter]);
	    logMessage(120, "Sample: %d; angle: %d; dist: %u; cm_array: %.2f; ping_array: %.2f \r\n", loop_counter, angle_array[loop_counter], dist_array[loop_counter], cm_array[loop_counter], ping_array[loop_counter]);
	    loop_counter++;
	}
	 */

    cyBOT_Scan(90, &scan);
    float ping1 = scan.sound_dist;
    timer_waitMicros(10000);
    cyBOT_Scan(90, &scan);
    float ping2 = scan.sound_dist;

    float ping_dist = (float) ((ping1 + ping2) / 2.0);
    uint16_t ir_avg = 0;


    uint16_t adc_array[16];
    int i;
    for(i = 0; i < 16; i++){
        adc_array[i] = adc_read();
        ir_avg += adc_array[i];
    }
    ir_avg = ir_avg / 16;
    float conv_dist = convertADCtoCM(ir_avg);
    logMessage(80, "ping_dist = %.2f; ir_avg = %u; conv_dist = %.2f\r\n", ping_dist, ir_avg, conv_dist);
    lcd_printf("IR_Value : %u \n Est dist: %f", ir_avg, conv_dist);

    uart_sendStr("End Sample!\r\n");
    return 0;
}
