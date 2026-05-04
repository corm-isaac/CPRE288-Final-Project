/*
 * adc.c
 *
 *  Created on: Mar 31, 2026
 *      Author: cooperrs
 */

#include "adc.h"

void adc_init (void)
{
    SYSCTL_RCGCADC_R |= 0x1; //activate ADC0
    SYSCTL_RCGCGPIO_R |= 0x2; // activate clock for port B
    while((SYSCTL_PRGPIO_R & 0x2) != 0x2){};

    GPIO_PORTB_DIR_R &= ~0x10; //make PB4 input
    GPIO_PORTB_AFSEL_R |= 0x10; // enable alt func for PB4
    GPIO_PORTB_DEN_R &= ~0x10; //disable digital io
    GPIO_PORTB_AMSEL_R |= 0x10; // enable analog func

    while((SYSCTL_PRADC_R & 0x1) != 0x1){};

    ADC0_ACTSS_R &= ~0x0008; // disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000; //  seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R |= 0xA; //  set channel 10
    ADC0_SSCTL3_R = 0x0006; // no TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0008; // disable SS3 interrupts
    ADC0_SAC_R |= 0x4; //Hardware averaging (16x)
    ADC0_ACTSS_R |= 0x0008; // enable sample sequencer 3
}

uint16_t adc_read (void)
{
    uint16_t result;
    ADC0_PSSI_R = 0x0008; // 1) initiate SS3

    while((ADC0_RIS_R&0x08) == 0){}; // 2) wait for conversion done

    result = ADC0_SSFIFO3_R & 0xFFF; // 3) read result
    ADC0_ISC_R = 0x0008; // 4)acknowledge completion
    return result;
}

float get_distance(uint16_t adc) {
    long EQ_CONST = 20527;
    float EQ_POWER_INV = -1.161440186; // 1/-0.857

    float adc_float = (float)adc;
    float return_val = pow(adc_float/EQ_CONST, EQ_POWER_INV);
    return return_val;

    //Process: Collect line of best fit, use form factor c*x^a (POWER) as format, where X is distance, and x is distance It looks great
    //If you did correct x and y for line of best fit, then done. If not, find the mathematical inverse so that your "x" is the raw ADC.
}
