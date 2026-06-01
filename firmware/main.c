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
    printf("Sistema iniciado\n");
    printf("\n");

    while(true)
    {
        uint16_t adc_raw =
            adc_read_raw();

        float percentual =
            adc_to_percent(adc_raw);

        float altura_cm =
            percent_to_height(percentual);

        float volume_ml =
            height_to_volume(altura_cm);

        printf(
            "ADC=%u  Nivel=%.1f%%  Altura=%.2f cm  Volume=%.0f mL\n",
            adc_raw,
            percentual,
            altura_cm,
            volume_ml
        );

        sleep_ms(LOOP_DELAY_MS);
    }
}
