/**
* @file timer.c
* @brief Handle time interrupts
*
* @author Michael Farmer
*
* @date 04/01/2026
*/
/**
* Interrupt Handler for timer edge received
* @author Michael Farmer
* @date 04/01/2026
*/
/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"

// Global shared variables
// Use extern declarations in the header file

volatile uint32_t timer_val_start = 0;
volatile uint32_t timer_val_end = 0;
volatile enum{LOW, HIGH, DONE} timer_state = LOW; // State of ping echo pulse
int overflows = 0;


void ping_init (void)
{

    SYSCTL_RCGCTIMER_R |= 0x8;
    SYSCTL_RCGCGPIO_R |= 0x2;
    while ((SYSCTL_PRTIMER_R & 0x8) == 0) {};
    while ((SYSCTL_PRGPIO_R & 0x2) == 0) {};

    //setup for trigger pulse
    GPIO_PORTB_DEN_R |= 0x8; //enable digital functions
    GPIO_PORTB_DIR_R |= 0x8; //set output
    GPIO_PORTB_AFSEL_R &= ~0x8; // disable af
    //GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~0xF000) | 0x7000;

    //TIMER INIT
    TIMER3_CTL_R &= ~0x100; //disable
    TIMER3_CFG_R = (TIMER3_CFG_R & ~0x7) | 0x4; //configure as 16 bit timer
    TIMER3_TBMR_R |= 0x7; // set to capture mode and edge time mode
    TIMER3_TBMR_R &= ~0x10; // set count direction
    TIMER3_CTL_R |= 0xC00; //set to rising and falling edge
    TIMER3_TBILR_R = 0xFFFF; // set max to count down from
    TIMER3_TBPR_R |= 0xFF; // set prescaler

    //timer interrupt things
    TIMER3_ICR_R |= 0x400; // clear capture event interrupt
    TIMER3_IMR_R |= 0x400; // set capture interrupt mask
    // is needed ? NVIC_PRI9_R = (NVIC_PRI9_R & ~0xE0) | 0x20; //interrupt 36 to priority 1
    NVIC_EN1_R |= 0x10; // interrupt 36

    timer_state = DONE;





    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    TIMER3_CTL_R |= 0x100; //enable timer
}

void ping_trigger (void)
{
    if (timer_state != DONE)
    {
        return;
    }
    timer_state = LOW;


    TIMER3_CTL_R &= ~0x100; // Disable timer
    //TIMER3_IMR_R &= ~0x400;
    GPIO_PORTB_PCTL_R &= ~0x7000; // reset port control
    GPIO_PORTB_AFSEL_R &= ~0x8;// Disable alternate function
    GPIO_PORTB_DIR_R |= 0x8; // set PB3 as digital output
    GPIO_PORTB_DATA_R |= 0x8; //set PB3 high

    timer_waitMicros(5);

    TIMER3_ICR_R |= 0x400; // Clear interrupts

    GPIO_PORTB_DATA_R &= ~0x8; //set PB3 low
    GPIO_PORTB_DIR_R &= ~0x8; //set digital input
    GPIO_PORTB_AFSEL_R |= 0x8; // Re-enable alternate function
    GPIO_PORTB_PCTL_R |= 0x7000; // set to use timer 3
    //TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;
}

void TIMER3B_Handler(void)
{
    //why do we do this?
    //uint32_t timer_val = 0xFFFFFF & TIMER3_TBR_R; //Lower 16

    if(timer_state == LOW)
    {
        timer_val_start = TIMER3_TBR_R;
        timer_state = HIGH;
    }
    else if (timer_state == HIGH)
    {
        timer_val_end = TIMER3_TBR_R;
        timer_state = DONE;
    }

    TIMER3_ICR_R |= 0x400;
}


uint32_t get_cycles(void)
{
    uint32_t cycles;
    //reminder we are counting down
    if(timer_val_end > timer_val_start) //if overflow
    {
        cycles = (0xFFFFFF - timer_val_end) + timer_val_start;
        overflows++;
    }
    else
    {
        cycles = timer_val_start - timer_val_end;
    }

    return cycles;
}

float ping_getDistance(uint32_t cycles)
{
    double pulse_dist = (double)cycles / 16000.0;
    float distance = (pulse_dist / 1000.0 / 2.0 * 34000.0); //why is it off by 2 every time?

    return distance - 2.0;

}
