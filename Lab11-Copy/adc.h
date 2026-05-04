/*
 * adc.h
 *
 *  Created on: Mar 31, 2026
 *      Author: cooperrs
 */

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

void adc_init(void);
uint16_t adc_read(void);

float get_distance(uint16_t adc);

#endif /* ADC_H_ */
