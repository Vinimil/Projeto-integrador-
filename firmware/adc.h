#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adc_setup(void);

uint16_t adc_read_raw(void);

#endif
