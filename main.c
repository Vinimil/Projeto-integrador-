#include <stdio.h>

#include "pico/stdlib.h"

#include "config.h"

#include "adc.h"

int main()
{
    stdio_init_all();

    sleep_ms(2000);

    adc_setup();

    printf("Sistema iniciado!\n\n");

    while(true)
    {
  
        // LEITURA ADC
        
        uint16_t adc_raw =
            adc_read_raw();


        // CONVERSÕES
        

        float nivel_percentual =
            adc_to_percent(adc_raw);

        float altura_cm =
            percent_to_height(
                nivel_percentual);

        float volume_ml =
            height_to_volume(
                altura_cm);

        // ==========================================
        // MISTURA
        // ==========================================

        float percentual_a =
            PERCENTUAL_A;

        float percentual_b =
            100.0f - percentual_a;

        float volume_a =
            VOLUME_TOTAL_ML *
            (percentual_a / 100.0f);

        float volume_b =
            VOLUME_TOTAL_ML *
            (percentual_b / 100.0f);

        // ==========================================
        // SERIAL MONITOR
        // ==========================================

        printf("ADC: %d\n",
               adc_raw);

        printf("Nivel: %.2f %%\n",
               nivel_percentual);

        printf("Altura: %.2f cm\n",
               altura_cm);

        printf("Volume tanque: %.2f mL\n",
               volume_ml);

        printf("Mistura total: %.2f mL\n",
               VOLUME_TOTAL_ML);

        printf("A: %.2f %% -> %.2f mL\n",
               percentual_a,
               volume_a);

        printf("B: %.2f %% -> %.2f mL\n\n",
               percentual_b,
               volume_b);

        sleep_ms(200);
    }
}
