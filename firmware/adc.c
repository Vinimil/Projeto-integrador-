#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "config.h"
#include "adc.h"

// INIT
void adc_setup()
{
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(ADC_CHANNEL);
}

// FILTRO 
uint16_t adc_read_raw()
{
    uint32_t soma = 0;

    for(int i = 0; i < NUM_AMOSTRAS; i++)
    {
        soma += adc_read();
        sleep_ms(5);
    }

    return soma / NUM_AMOSTRAS;
}

// ADC -> %
float adc_to_percent(uint16_t adc_value)
{
    float p =
        ((float)(adc_value - ADC_MIN) /
        (ADC_MAX - ADC_MIN)) * 100.0f;

    if(p < 0) p = 0;
    if(p > 100) p = 100;

    return p;
}

// % -> altura
float percent_to_height(float percent)
{
    return (percent / 100.0f) * TANQUE_ALTURA_CM;
}

// altura -> volume
float height_to_volume(float h)
{
    return TANQUE_COMPR_CM * TANQUE_LARGURA_CM * h * 10.0f;
}
