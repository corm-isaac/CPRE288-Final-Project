/*
 * adc.c
 *
 *  Created on: Mar 25, 2026
 *      Author: ikcorm
 */
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart-interrupt.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

void adc_init (void){
    SYSCTL_RCGCADC_R |= 0x0001; //activate ADC0
    SYSCTL_RCGCGPIO_R |= 0x02; //Port B init
    while((SYSCTL_PRGPIO_R&0x02) != 0x02){};
    GPIO_PORTB_DIR_R &= ~0x10; //GPIO Init
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    ADC0_ACTSS_R &= ~0x01; //disable SS0
    ADC0_EMUX_R &= ~0x000F; //sets SS3 to contionusly sample
    ADC0_SSMUX0_R &= ~0x000F;
    ADC0_SSMUX0_R += 10; //
    ADC0_SSCTL0_R = 0x0006;
    ADC0_IM_R &= ~0x0001; //? disables interrupts SS0
    ADC0_ACTSS_R |= 0x0001; //enable SS0
}
uint16_t adc_read (void){
    uint16_t result;
    ADC0_PSSI_R = 0x0001; //ss0
    while ((ADC0_RIS_R & 0x01)==0){};
    result = ADC0_SSFIFO0_R & 0xFFF;
    ADC0_ISC_R = 0x0001;
    return result;
}



