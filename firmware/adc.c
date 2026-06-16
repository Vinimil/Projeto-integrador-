#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "config.h"
#include "adc.h"

void adc_setup(void)
{
    adc_init();

    adc_gpio_init(ADC_PIN);

    adc_select_input(ADC_CHANNEL);
}

uint16_t adc_read_raw(void)
{
    uint32_t soma = 0;

    for(int i = 0; i < NUM_AMOSTRAS; i++)
    {
        soma += adc_read();
    }

    return soma / NUM_AMOSTRAS;
}
