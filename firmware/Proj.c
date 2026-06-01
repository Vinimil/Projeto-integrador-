#include <stdio.h>
#include "pico/stdlib.h"

#include "config.h"
#include "adc.h"

int main()
{
    stdio_init_all();

    adc_setup();

    sleep_ms(2000);

    printf("\n");
    
    printf(" MONITOR DE NIVEL DO TANQUE\n");
 

    while (true)
    {
        uint16_t adc_raw = adc_read_raw();

        float nivel = adc_to_percent(adc_raw);

        float altura = percent_to_height(nivel);

        float volume = height_to_volume(altura);

        printf("ADC     : %u\n", adc_raw);
        printf("Nivel   : %.1f %%\n", nivel);
        printf("Altura  : %.2f cm\n", altura);
        printf("Volume  : %.1f mL\n", volume);

        printf("-----------------------------\n");

        sleep_ms(500);
    }

    return 0;
}
