#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "config.h"
#include "adc.h"


//  BARRA

void print_bar(float percent)
{
    int size = 20;
    int filled = (int)((percent / 100.0f) * size);

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


// Interface

void print_menu(float nivel, float volume, int dose)
{
    printf("\n====================================\n");
    printf("        DOSADOR AUTOMATICO\n");
    printf("====================================\n\n");

    printf("Nivel tanque : %.1f %%\n", nivel);
    printf("Volume       : %.1f mL\n\n", volume);

    printf("Dose atual   : %d mL\n\n", dose);

    printf("[+] aumentar\n");
    printf("[-] diminuir\n");
    printf("[s] iniciar\n");
    printf("[r] refresh\n");
    printf("[q] reset\n\n");

    printf("> ");
}


// Dosagem 

void dosar(int dose)
{
    printf("\n====================================\n");
    printf("             DOSANDO\n");
    printf("====================================\n\n");

    int atual = 0;

    while(atual < dose)
    {
        atual += 50;
        if(atual > dose) atual = dose;

        float p = ((float)atual / dose) * 100.0f;

        printf("Dose: %d / %d mL\n", atual, dose);
        print_bar(p);

        sleep_ms(300);
    }

    printf("\nDOSAGEM CONCLUIDA\n\n");
}

int main()
{
    stdio_init_all();
    adc_setup();

    int dose = 700;

    sleep_ms(2000);

    while(true)
    {
        uint16_t raw = adc_read_raw();

        float nivel = adc_to_percent(raw);
        float altura = percent_to_height(nivel);
        float volume = height_to_volume(altura);

        print_menu(nivel, volume, dose);

        char c = getchar_timeout_us(0);

        if(c == PICO_ERROR_TIMEOUT)
        {
            sleep_ms(200);
            continue;
        }

        if(c == '+')
            dose += DOSE_STEP;

        if(c == '-')
            dose -= DOSE_STEP;

        if(dose > DOSE_MAX_ML) dose = DOSE_MAX_ML;
        if(dose < DOSE_MIN_ML) dose = DOSE_MIN_ML;

        if(c == 's')
        {
            dosar(dose);
        }

        if(c == 'q')
        {
            dose = 700;
        }

        sleep_ms(200);
    }
}
