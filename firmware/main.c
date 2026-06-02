#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "config.h"
#include "adc.h"


// BOMBA


void bomba_init()
{
    gpio_init(BOMBA_PIN);
    gpio_set_dir(BOMBA_PIN, GPIO_OUT);

    gpio_put(BOMBA_PIN, 0);
}

void bomba_ligar()
{
    gpio_put(BOMBA_PIN, 1);
}

void bomba_desligar()
{
    gpio_put(BOMBA_PIN, 0);
}



void print_bar(float percent)
{
    int size = 20;

    int filled =
        (int)((percent / 100.0f) * size);

    printf("[");

    for(int i = 0; i < size; i++)
    {
        if(i < filled)
            printf("#");
        else
            printf("-");
    }

    printf("] %.1f%%\n", percent);
}


// MENU


void print_menu(
    float nivel,
    float volume,
    int dose)
{
    printf("\n");
    printf("====================================\n");
    printf("        DOSADOR AUTOMATICO\n");
    printf("====================================\n\n");

    printf("Nivel tanque : %.1f %%\n", nivel);
    printf("Volume       : %.0f mL\n\n", volume);

    printf("Dose atual   : %d mL\n\n", dose);

    printf("[+] aumentar dose\n");
    printf("[-] diminuir dose\n");
    printf("[s] iniciar dosagem\n");
    printf("[r] atualizar\n");
    printf("[q] reset\n\n");

    printf("> ");
}


// Dosagem

void dosar(float volume_disponivel, int dose_ml)
{
    if(volume_disponivel < dose_ml)
    {
        printf("\nERRO: Tanque sem volume suficiente.\n");
        return;
    }

    float volume_corrigido =
        dose_ml - COMPENSACAO_ML;

    if(volume_corrigido < 0)
        volume_corrigido = 0;

    float tempo_segundos =
        volume_corrigido /
        VAZAO_BOMBA_ML_S;

    uint32_t tempo_ms =
        (uint32_t)(tempo_segundos * 1000.0f);

    if(tempo_ms < TEMPO_MIN_BOMBA_MS)
        tempo_ms = TEMPO_MIN_BOMBA_MS;

    printf("\n");
    printf("====================================\n");
    printf("            DOSANDO\n");
    printf("====================================\n\n");

    printf("Dose solicitada : %d mL\n", dose_ml);
    printf("Compensacao     : %.1f mL\n", COMPENSACAO_ML);
    printf("Tempo calculado : %lu ms\n", tempo_ms);

    bomba_ligar();

    uint32_t inicio = to_ms_since_boot(get_absolute_time());

    while(true)
    {
        uint32_t agora =
            to_ms_since_boot(
                get_absolute_time());

        uint32_t decorrido =
            agora - inicio;

        float percentual =
            ((float)decorrido /
            (float)tempo_ms) * 100.0f;

        if(percentual > 100.0f)
            percentual = 100.0f;

        printf("\r");
        print_bar(percentual);

        if(decorrido >= tempo_ms)
            break;

        sleep_ms(100);
    }

    bomba_desligar();

    printf("\n");
    printf("DOSAGEM CONCLUIDA\n");
    printf("\n");
}


int main()
{
    stdio_init_all();

    adc_setup();

    bomba_init();

    int dose = 700;

    sleep_ms(2000);

    while(true)
    {
        uint16_t raw =
            adc_read_raw();

        float nivel =
            adc_to_percent(raw);

        float altura =
            percent_to_height(nivel);

        float volume =
            height_to_volume(altura);

        print_menu(
            nivel,
            volume,
            dose
        );

        int c =
            getchar_timeout_us(0);

        if(c == PICO_ERROR_TIMEOUT)
        {
            sleep_ms(200);
            continue;
        }

        if(c == '+')
        {
            dose += DOSE_STEP_ML;
        }

        if(c == '-')
        {
            dose -= DOSE_STEP_ML;
        }

        if(dose > DOSE_MAX_ML)
            dose = DOSE_MAX_ML;

        if(dose < DOSE_MIN_ML)
            dose = DOSE_MIN_ML;

        if(c == 's')
        {
            dosar(volume, dose);
        }

        if(c == 'q')
        {
            dose = 700;
        }

        sleep_ms(200);
    }

    return 0;
}
