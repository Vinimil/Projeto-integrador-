#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "config.h"
#include "adc.h"


// CONVERTE % - ALTURA


float percentual_para_altura(float percentual)
{
    return (percentual / 100.0f) * TANQUE_ALTURA_CM;
}


// ALTURA - VOLUME


float altura_para_volume(float altura_cm)
{
    return altura_cm *
           TANQUE_LARGURA_CM *
           TANQUE_COMPR_CM;
}

int main()
{
    stdio_init_all();

    adc_setup();

  
    // CONFIGURA BOMBAS
   

    gpio_init(BOMBA_A_PIN);
    gpio_set_dir(BOMBA_A_PIN, GPIO_OUT);

    gpio_init(BOMBA_B_PIN);
    gpio_set_dir(BOMBA_B_PIN, GPIO_OUT);

    gpio_put(BOMBA_A_PIN, 0);
    gpio_put(BOMBA_B_PIN, 0);

    sleep_ms(2000);


    // LEITURA INICIAL


    float nivel_inicial_A =
        adc_para_percentual(
            adc_ler_filtrado(ADC_CHANNEL_A),
            ADC_MIN_A,
            ADC_MAX_A);

    float nivel_inicial_B =
        adc_para_percentual(
            adc_ler_filtrado(ADC_CHANNEL_B),
            ADC_MIN_B,
            ADC_MAX_B);

 
    // CÁLCULOS
    
    float percentual_B = 100.0f - PERCENTUAL_A;

    float volume_A_desejado =
        VOLUME_TOTAL_ML * (PERCENTUAL_A / 100.0f);

    float volume_B_desejado =
        VOLUME_TOTAL_ML * (percentual_B / 100.0f);

    printf("\n=== MISTURA ===\n");

    printf("A: %.1f mL\n", volume_A_desejado);
    printf("B: %.1f mL\n", volume_B_desejado);

   
    // LIGA BOMBAS
    

    gpio_put(BOMBA_A_PIN, 1);
    gpio_put(BOMBA_B_PIN, 1);

    bool bomba_A_ligada = true;
    bool bomba_B_ligada = true;


    // LOOP
 

    while (1)
    {
     
        // LEITURA TANQUE A
     
        float nivel_A =
            adc_para_percentual(
                adc_ler_filtrado(ADC_CHANNEL_A),
                ADC_MIN_A,
                ADC_MAX_A);

  
        // LEITURA TANQUE B
    

        float nivel_B =
            adc_para_percentual(
                adc_ler_filtrado(ADC_CHANNEL_B),
                ADC_MIN_B,
                ADC_MAX_B);


        // CONVERTE PARA VOLUME


        float altura_A =
            percentual_para_altura(nivel_A);

        float altura_B =
            percentual_para_altura(nivel_B);

        float volume_A_atual =
            altura_para_volume(altura_A);

        float volume_B_atual =
            altura_para_volume(altura_B);


        // VOLUME INICIAL


        float altura_inicial_A =
            percentual_para_altura(nivel_inicial_A);

        float altura_inicial_B =
            percentual_para_altura(nivel_inicial_B);

        float volume_A_inicial =
            altura_para_volume(altura_inicial_A);

        float volume_B_inicial =
            altura_para_volume(altura_inicial_B);

   
        // QUANTO JÁ SAIU
        
        float volume_saida_A =
            volume_A_inicial - volume_A_atual;

        float volume_saida_B =
            volume_B_inicial - volume_B_atual;

    
        // DESLIGA BOMBAS


        if (bomba_A_ligada &&
            volume_saida_A >= volume_A_desejado)
        {
            gpio_put(BOMBA_A_PIN, 0);

            bomba_A_ligada = false;

            printf("Bomba A desligada\n");
        }

        if (bomba_B_ligada &&
            volume_saida_B >= volume_B_desejado)
        {
            gpio_put(BOMBA_B_PIN, 0);

            bomba_B_ligada = false;

            printf("Bomba B desligada\n");
        }


        // MONITOR SERIAL
    

        printf("\n");

        printf("A -> %.1f mL / %.1f mL\n",
               volume_saida_A,
               volume_A_desejado);

        printf("B -> %.1f mL / %.1f mL\n",
               volume_saida_B,
               volume_B_desejado);


        // FINALIZA


        if (!bomba_A_ligada &&
            !bomba_B_ligada)
        {
            printf("\nMistura concluida!\n");

            break;
        }

        sleep_ms(LOOP_DELAY_MS);
    }

    while (1)
    {
        tight_loop_contents();
    }
}
