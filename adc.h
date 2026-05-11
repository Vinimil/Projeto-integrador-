#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adc_setup(void);

uint16_t adc_read_filtered(void);

float adc_to_voltage(uint16_t adc_value);

float adc_to_percent(uint16_t adc_value);

#endif