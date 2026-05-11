#include "adc.h"
#include "config.h"

#include <stdint.h>

#include "hardware/adc.h"
#include "pico/stdlib.h"

void adc_setup(void)
{
    adc_init();

    adc_gpio_init(ADC_PIN);

    adc_select_input(ADC_CHANNEL);
}

uint16_t adc_read_filtered(void)
{
    uint32_t soma = 0;

    for(int i = 0; i < NUM_AMOSTRAS; i++)
    {
        soma += adc_read();

        sleep_ms(2);
    }

    return soma / NUM_AMOSTRAS;
}

float adc_to_voltage(uint16_t adc_value)
{
    return (adc_value * ADC_VREF) / ADC_RESOLUTION;
}

float adc_to_percent(uint16_t adc_value)
{
    float nivel;

    nivel = ((adc_value - ADC_MIN) * 100.0f) /
             (ADC_MAX - ADC_MIN);

    if(nivel < 0)
    {
        nivel = 0;
    }

    if(nivel > 100)
    {
        nivel = 100;
    }

    return nivel;
}