/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"
#include "math.h"
#include "lcd.h"


// Global shared variables
// Use extern declarations in the header file

volatile uint32_t overflow_occurence = 0;

volatile uint32_t g_start_time = 0;
volatile uint32_t g_end_time = 0;
volatile enum{LOW, HIGH, DONE} g_state = LOW; // State of ping echo pulse

void ping_init (void){

  // YOUR CODE HERE
    SYSCTL_RCGCGPIO_R |= 0b00000010; //Port B
    SYSCTL_RCGCTIMER_R |= 0x08; //Timer 3

    while ((SYSCTL_PRGPIO_R & 0b000000010) == 0) {}; // busy wait for PRGPIO
    //while ((SYSCTL_PRTIMER_R & 0x08) == 0) {}; //busy wait for timer

    GPIO_PORTB_DIR_R &= ~0x08; //PB3 is set as in input
    GPIO_PORTB_DEN_R |= 0x08; //enables PB3

    GPIO_PORTB_AFSEL_R |= 0x08; //PB3 is alt function
    GPIO_PORTB_PCTL_R &= ~0x0000F000;
    GPIO_PORTB_PCTL_R |= 0x00007000; //14:12 is marked 1 (PMC3)

    TIMER3_CTL_R &= ~0x100; //sets every register value 0 except for TBEN

    TIMER3_CFG_R = 0x4; //For a 16/32-bit timer, this value selects the 16-bit timer configuration.

    TIMER3_TBMR_R = ((TIMER3_TBMR_R & ~0x03) | 0x03); //
    TIMER3_TBMR_R |= 0x04;
    TIMER3_TBMR_R &= ~0x10;

    TIMER3_CTL_R |= 0x0C00;

    TIMER3_TBPR_R |= 0xFF;

    TIMER3_TBILR_R |= 0xFFFF;

    TIMER3_IMR_R |= 0x400;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    //TIMER3_ICR_R |= 0x400;
    NVIC_EN1_R |= 0x10;

    //NVIC_PRI9_R |= 0x10;


    IntMasterEnable();

    // Configure and enable the timer
    TIMER3_CTL_R |= 0x100;
}

void ping_trigger (void){
    g_state = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x0100;
    TIMER3_IMR_R &= ~0x0400; //match interrupt mask?? TBMMIS for GPTM for the MIS
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_DEN_R |= 0x08;

    GPIO_PORTB_DATA_R &= ~0x08;
    GPIO_PORTB_DATA_R |= 0x08;

    timer_waitMicros(5);

    GPIO_PORTB_DATA_R &= ~0x08;

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x0400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0x0400;
    TIMER3_CTL_R |= 0x0100;
}

void TIMER3B_Handler(void){

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.



     //char byte_received;
    //check if handler called due to RX event


    if(TIMER3_MIS_R & 0x0400){

        TIMER3_ICR_R = 0x400;

        if(g_state == LOW){
            g_end_time = TIMER3_TBR_R;
            g_state = HIGH;
        }
        else if (g_state == HIGH){
            g_start_time = TIMER3_TBR_R;
            g_state = DONE;
        }
    }


    TIMER3_ICR_R |= 0x400; //clear timer

}

float ping_getDistance (void){

    float echoPulseWidth = 0;
    float time;
    float dist;

    //ping_trigger();

    while(g_state != DONE) {};

    if(g_start_time < g_end_time){
        echoPulseWidth = 0xFFFFF - g_end_time + g_start_time;
        //Overflow
        overflow_occurence++;
    }
    else {
        echoPulseWidth = g_start_time - g_end_time;
    }

    time = echoPulseWidth * (1.0/16000000.0);
    dist = (((343 * time) / 2) * 100);
    return dist;

}
