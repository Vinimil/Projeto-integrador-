#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"

#include "config.h"
#include "adc.h"

int main()
{
    stdio_init_all();

    adc_setup();

    float nivel_anterior = -100.0f;

    while(true)
    {
        uint16_t adc_raw = adc_read_filtered();

        float tensao = adc_to_voltage(adc_raw);

        float nivel = adc_to_percent(adc_raw);

        if((nivel > (nivel_anterior + HISTERESE_PERCENT)) ||
           (nivel < (nivel_anterior - HISTERESE_PERCENT)))
        {
            nivel_anterior = nivel;
        }

        printf("\n");

        printf("ADC RAW: %d\n", adc_raw);

        printf("Tensao: %.2f V\n", tensao);

        printf("Nivel: %.1f %%\n", nivel_anterior);

        printf("--------------------\n");

        sleep_ms(TEMPO_LEITURA_MS);
    }

    return 0;
}