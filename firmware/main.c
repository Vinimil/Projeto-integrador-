#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "hardware/gpio.h"

#include "config.h"
#include "adc.h"
#include "calibracao.h"

void bomba_setup(void)
{
    gpio_init(BOMBA_PIN);
    gpio_set_dir(BOMBA_PIN, GPIO_OUT);
    gpio_put(BOMBA_PIN, 0);
}

void bomba_ligar(void)
{
    gpio_put(BOMBA_PIN, 1);
}

void bomba_desligar(void)
{
    gpio_put(BOMBA_PIN, 0);
}

float ler_volume_filtrado(void)
{
    float soma = 0;

    for(int i = 0; i < FILTRO_LEITURAS; i++)
    {
        soma += adc_para_volume(adc_read_raw());
        sleep_ms(5);
    }

    return soma / FILTRO_LEITURAS;
}

void aplicar_pulso(uint32_t tempo_ms)
{
    bomba_ligar();
    sleep_ms(tempo_ms);
    bomba_desligar();
}

void dosar(float dose_ml)
{
    float fator;

    if(dose_ml >= 300)
    {
        fator = 1.45f;
    }
    else
    {
        fator = 1.18f;
    }

    float volume_inicial = ler_volume_filtrado();

    printf("\n====================================\n");
    printf("INICIANDO DOSAGEM\n");
    printf("====================================\n");
    printf("Volume inicial : %.0f mL\n", volume_inicial);
    printf("Dose pedida    : %.0f mL\n", dose_ml);
    printf("Fator usado    : %.2f\n", fator);
    printf("====================================\n");

    float ultimo_volume = volume_inicial;
    int sem_mudanca = 0;

    while(true)
    {
        float volume = ler_volume_filtrado();

        float retirado_sensor =
            volume_inicial - volume;

        float retirado =
            retirado_sensor * fator;

        float restante =
            dose_ml - retirado;

        if(restante <= 0)
        {
            printf("\nALVO ATINGIDO\n");
            break;
        }

        uint32_t pulso;

        if(restante > 180)
        {
            pulso = 1200;
        }
        else if(restante > 50)
        {
            pulso = 700;
        }
        else
        {
            pulso = 300;
        }

        aplicar_pulso(pulso);

        sleep_ms(TEMPO_ESTABILIZAR_MS);

        uint16_t adc = adc_read_raw();

        volume = ler_volume_filtrado();

        retirado_sensor =
            volume_inicial - volume;

        retirado =
            retirado_sensor * fator;

        restante =
            dose_ml - retirado;

        printf(
            "ADC=%u  Volume=%.0f  Retirado=%.0f  Restante=%.0f  Pulso=%lu\n",
            adc,
            volume,
            retirado,
            restante,
            (unsigned long)pulso
        );

        if(fabs(volume - ultimo_volume) < 3.0f)
        {
            sem_mudanca++;
        }
        else
        {
            sem_mudanca = 0;
        }

        ultimo_volume = volume;

        if(sem_mudanca >= MAX_SEM_MUDANCA)
        {
            printf("\nERRO: SENSOR SEM RESPOSTA\n");
            return;
        }
    }

    sleep_ms(2000);

    float volume_final =
        ler_volume_filtrado();

    float retirado_sensor =
        volume_inicial - volume_final;

    float retirado_estimado =
        retirado_sensor * fator;

    printf("\n====================================\n");
    printf("RESULTADO\n");
    printf("====================================\n");
    printf("Pedido       : %.0f mL\n", dose_ml);
    printf("Inicial      : %.0f mL\n", volume_inicial);
    printf("Final        : %.0f mL\n", volume_final);
    printf("Sensor       : %.0f mL\n", retirado_sensor);
    printf("Estimado     : %.0f mL\n", retirado_estimado);
    printf("====================================\n");
}

int main()
{
    stdio_init_all();

    adc_setup();
    bomba_setup();

    while(!stdio_usb_connected())
    {
        sleep_ms(100);
    }

    sleep_ms(1000);

    while(true)
    {
        uint16_t adc =
            adc_read_raw();

        float volume =
            ler_volume_filtrado();

        printf("\n====================================\n");
        printf("SISTEMA PRONTO\n");
        printf("====================================\n");
        printf("ADC atual    : %u\n", adc);
        printf("Volume atual : %.0f mL\n", volume);
        printf("====================================\n");
        printf("1 - 250 mL\n");
        printf("2 - 300 mL\n");
        printf("3 - 350 mL\n");
        printf("Escolha: ");

        char opcao;

        scanf(" %c", &opcao);

        switch(opcao)
        {
            case '1':
                dosar(250);
                break;

            case '2':
                dosar(300);
                break;

            case '3':
                dosar(350);
                break;

            default:
                printf("Opcao invalida\n");
                break;
        }
    }

    return 0;
}
