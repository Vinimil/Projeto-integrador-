#include <stdio.h>

#include "pico/stdlib.h"

#include "hardware/adc.h"

#include "config.h"

#include "adc.h"


// Inicializa ADC

void adc_setup()
{
    adc_init();

    adc_gpio_init(ADC_PIN);

    adc_select_input(ADC_CHANNEL);
}


// Leitura média

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


// ADC para porcentagem


float adc_to_percent(uint16_t adc_value)
{
    float percent =
        ((float)(adc_value - ADC_MIN) /
        (ADC_MAX - ADC_MIN)) * 100.0f;

    if(percent < 0.0f)
        percent = 0.0f;

    if(percent > 100.0f)
        percent = 100.0f;

    return percent;
}


// porcentagem para altura


float percent_to_height(float percent)
{
    return
        (percent / 100.0f) *
        TANQUE_ALTURA_CM;
}


// altura para volume


float height_to_volume(float height_cm)
{
    return
        TANQUE_COMPRIMENTO_CM *
        TANQUE_LARGURA_CM *
        height_cm;
}
