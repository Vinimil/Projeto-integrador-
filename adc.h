#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adc_setup();

uint16_t adc_read_raw();

float adc_to_percent(uint16_t adc_value);

float percent_to_height(float percent);

float height_to_volume(float height_cm);

#endif
