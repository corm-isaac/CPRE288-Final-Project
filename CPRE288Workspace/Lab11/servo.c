/*
 * servo.c
 *
 *  Created on: Apr 7, 2026
 *      Author: cooperrs
 */
#include "servo.h"
#include "adc.h"
#include "ping.h"


void servo_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x2; //enable clock for gpioB
    SYSCTL_RCGCTIMER_R |= 0x02; //enable clock for timer 1
    GPIO_PORTB_DEN_R |= 0x20; //enable digital for PB5
    GPIO_PORTB_DIR_R |= 0x20; //set PB5 as output
    GPIO_PORTB_AFSEL_R |= 0x20; //enable alt function for PB5
    GPIO_PORTB_PCTL_R |= 0x700000; //set PB5 digital functino to T1CCP1

    TIMER1_CTL_R &= ~0x100; //disable timer 1B
    TIMER1_CFG_R = 0x04; //set timer to 16bit
    TIMER1_TBMR_R = 0xA; //pwm, edge-count, perodic
    TIMER1_TBILR_R = 0xE200; //load 65535 as count down start
    TIMER1_TBPR_R = 0x4; //set 8bit prescaler
    TIMER1_TBMATCHR_R = 0x9082;
    TIMER1_TBPMR_R = 0x4;
    TIMER1_CTL_R |= 0x100; //enable timer
}

void servo_move(float degrees)
{
    /*
    cybot 11
    int r_cal = 313202;
    int l_cal = 284918;

    cybot 23
    int r_cal = 311638;
    int l_cal = 284446;
     */

    int r_cal = 311638;
    int l_cal = 284446;
    int match = r_cal + (-1 * ((r_cal - l_cal) / 180.0) * degrees); //gives 5 hex
    TIMER1_TBMATCHR_R = match & 0xFFFF; //load with lower 4 hex
    TIMER1_TBPMR_R = (match >> 16); //cut off lower 4 hex
    timer_waitMillis(30);
}


// init lcd, button, and timer before calling
void servo_calibrate(void)
{
    uint8_t curr_button = 0;

    //comply with part 2-2
    int servo_pos = 90;
    servo_move(servo_pos);
    bool clockwise = false;
    int queued_button = -1; //0 = none, else 1-4; //for falling edge detection

    while(1)
    {
        curr_button = button_getButton();
        if(clockwise == true) //turn right
        {
            if(curr_button == 1)
            {
                queued_button = 1;
            }
            else if(curr_button == 2)
            {
                queued_button = 2;
            }
            else if (curr_button == 3)
            {
                queued_button = 3;
            }
            else if (curr_button == 4)
            {
                queued_button = 4;
            }


            else if(queued_button == 1) {
                queued_button = 0;

                servo_pos -= 1;
                servo_move(servo_pos);
            }
            else if(queued_button == 2) {
                queued_button = 0;

                servo_pos -= 5;
                servo_move(servo_pos);
            }
            else if(queued_button == 3) {
                queued_button = 0;

                clockwise = false;
            }
            else if(queued_button == 4) {
                queued_button = 0;

                servo_pos = 5;
                servo_move(servo_pos);
            }

            unsigned int timer_match = (TIMER1_TBPMR_R << 16) | TIMER1_TBMATCHR_R;
            lcd_printf("clockwise\ncurrent: %d\ndegrees: %d", timer_match, servo_pos);
        }
        else // counter clock wise
        {
            if(curr_button == 1)
            {
                queued_button = 1;
            }
            else if(curr_button == 2)
            {
                queued_button = 2;
            }
            else if (curr_button == 3)
            {
                queued_button = 3;
            }
            else if (curr_button == 4)
            {
                queued_button = 4;
            }


            else if(queued_button == 1) {
                queued_button = 0;

                servo_pos += 1;
                servo_move(servo_pos);
            }
            else if(queued_button == 2) {
                queued_button = 0;

                servo_pos += 5;
                servo_move(servo_pos);
            }
            else if(queued_button == 3) {
                queued_button = 0;

                clockwise = true;
            }
            else if(queued_button == 4) {
                queued_button = 0;

                servo_pos = 175;
                servo_move(servo_pos);
            }
            unsigned int timer_match = (TIMER1_TBPMR_R << 16) + TIMER1_TBMATCHR_R;
            lcd_printf("counter clockwise\ncurrent: %d\ndegrees: %d", timer_match, servo_pos);
        }
    }

}

uint16_t IRScan(float degrees){
    servo_move(degrees);
    int i;
    int sum = 0;
    for(i = 0; i < 3; i++){
        sum += adc_read();
    }
    return get_distance((sum / 3));
    //return adc_read();
}

float PingScan(int degrees){
    servo_move(degrees);
    ping_trigger();

    return ping_getDistance(get_cycles());

}



