#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "hardware/gpio.h"

#include "config.h"
#include "adc.h"
#include "calibracao.h"
#include "lcd_i2c.h"

float obter_fator(float dose_ml)
{
    if(dose_ml <= 250)
        return 1.22f;

    return 1.44f;
}

float adc_para_tensao(uint16_t adc)
{
    return (adc * 3.3f) / 4095.0f;
}

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

void mostrar_tela_principal(void)
{
    uint16_t adc = adc_read_raw();

    float tensao =
        adc_para_tensao(adc);

    float volume =
        ler_volume_filtrado();

    char linha1[17];
    char linha2[17];

    snprintf(linha1,
             sizeof(linha1),
             "Vol:%4.0fmL",
             volume);

    snprintf(linha2,
             sizeof(linha2),
             "T:%1.2fV",
             tensao);

    lcd_clear();

    lcd_set_cursor(0,0);
    lcd_print(linha1);

    lcd_set_cursor(0,1);
    lcd_print(linha2);
}

void modo_calibracao(void)
{
    char entrada[32];

    printf("\n");
    printf("====================================\n");
    printf("MODO CALIBRACAO\n");
    printf("====================================\n");
    printf("Digite -1 para sair\n\n");

    while(true)
    {
        uint16_t adc =
            adc_read_raw();

        float tensao =
            adc_para_tensao(adc);

        float volume_atual =
            adc_para_volume(adc);

        lcd_clear();

        char linha1[17];
        char linha2[17];

        snprintf(linha1,
                 sizeof(linha1),
                 "T:%1.2fV",
                 tensao);

        snprintf(linha2,
                 sizeof(linha2),
                 "Vol:%4.0fmL",
                 volume_atual);

        lcd_set_cursor(0,0);
        lcd_print(linha1);

        lcd_set_cursor(0,1);
        lcd_print(linha2);

        printf("\n");
        printf("------------------------------------\n");
        printf("ADC    : %u\n", adc);
        printf("Tensao : %.3f V\n", tensao);
        printf("------------------------------------\n");
        printf("Volume (mL): ");

        scanf("%31s", entrada);

        float volume =
            atof(entrada);

        if(volume < 0)
        {
            printf("\nSaindo da calibracao...\n");
            break;
        }

        adc = adc_read_raw();

        printf("\n");
        printf("ADC = %u -> %.0f mL\n",
               adc,
               volume);

        printf("Tabela para copiar:\n");
        printf("%u,\n", adc);
    }
}

void dosar(float dose_ml)
{
    float fator =
        obter_fator(dose_ml);

    uint16_t adc_inicial =
        adc_read_raw();

    float volume_inicial =
        ler_volume_filtrado();

    printf("\n====================================\n");
    printf("INICIANDO DOSAGEM\n");
    printf("====================================\n");
    printf("Tensao inicial : %.2f V\n",
           adc_para_tensao(adc_inicial));
    printf("Volume inicial : %.0f mL\n",
           volume_inicial);
    printf("Dose pedida    : %.0f mL\n",
           dose_ml);
    printf("====================================\n");

    float ultimo_volume =
        volume_inicial;

    int sem_mudanca = 0;

    while(true)
    {
        float volume =
            ler_volume_filtrado();

        float retirado_sensor =
            volume_inicial - volume;

        float retirado =
            retirado_sensor * fator;

        float restante =
            dose_ml - retirado;

        char linha1[17];
        char linha2[17];

        snprintf(linha1,
                 sizeof(linha1),
                 "Dose %.0fmL",
                 dose_ml);

        snprintf(linha2,
                 sizeof(linha2),
                 "Falta %.0f",
                 restante > 0 ? restante : 0);

        lcd_clear();

        lcd_set_cursor(0,0);
        lcd_print(linha1);

        lcd_set_cursor(0,1);
        lcd_print(linha2);

        if(restante <= 0)
            break;

        uint32_t pulso;

        if(restante > 180)
            pulso = PULSO_LONGO_MS;
        else if(restante > 50)
            pulso = PULSO_MEDIO_MS;
        else
            pulso = PULSO_CURTO_MS;

        aplicar_pulso(pulso);

        sleep_ms(TEMPO_ESTABILIZAR_MS);

        uint16_t adc =
            adc_read_raw();

        volume =
            ler_volume_filtrado();

        retirado_sensor =
            volume_inicial - volume;

        retirado =
            retirado_sensor * fator;

        restante =
            dose_ml - retirado;

        printf("T=%.2fV  Vol=%.0fmL  Falta=%.0fmL\n",
               adc_para_tensao(adc),
               volume,
               restante);

        if(fabs(volume - ultimo_volume) < 3.0f)
            sem_mudanca++;
        else
            sem_mudanca = 0;

        ultimo_volume = volume;

        if(sem_mudanca >= MAX_SEM_MUDANCA)
        {
            lcd_clear();

            lcd_set_cursor(0,0);
            lcd_print("ERRO SENSOR");

            lcd_set_cursor(0,1);
            lcd_print("Sem resposta");

            bomba_desligar();

            printf("ERRO SENSOR\n");

            return;
        }
    }

    sleep_ms(1500);

    uint16_t adc_final =
        adc_read_raw();

    float volume_final =
        ler_volume_filtrado();

    printf("\n====================================\n");
    printf("DOSAGEM FINALIZADA\n");
    printf("====================================\n");
    printf("Tensao final  : %.2f V\n",
           adc_para_tensao(adc_final));
    printf("Volume final  : %.0f mL\n",
           volume_final);
    printf("====================================\n");

    lcd_clear();

    lcd_set_cursor(0,0);
    lcd_print("Dose concluida");

    char resultado[17];

    snprintf(resultado,
             sizeof(resultado),
             "%.0fmL OK",
             dose_ml);

    lcd_set_cursor(0,1);
    lcd_print(resultado);

    sleep_ms(3000);
}

int main()
{
    stdio_init_all();

    adc_setup();
    bomba_setup();
    lcd_init_display();

    while(!stdio_usb_connected())
    {
        sleep_ms(100);
    }

    sleep_ms(1000);

    printf("\n");
    printf("====================================\n");
    printf(" SISTEMA DE DOSAGEM AUTOMATICA\n");
    printf("====================================\n");

    while(true)
    {
        mostrar_tela_principal();

        uint16_t adc =
            adc_read_raw();

        float volume =
            ler_volume_filtrado();

        float tensao =
            adc_para_tensao(adc);

        printf("\n");
        printf("====================================\n");
        printf("MENU PRINCIPAL\n");
        printf("====================================\n");
        printf("Volume : %.0f mL\n", volume);
        printf("Tensao : %.2f V\n", tensao);
        printf("====================================\n");
        printf("1 - Calibracao\n");
        printf("2 - Dosar 250 mL\n");
        printf("3 - Dosar 300 mL\n");
        printf("4 - Dosar 350 mL\n");
        printf("Escolha: ");

        char opcao;

        scanf(" %c", &opcao);

        switch(opcao)
        {
            case '1':
                modo_calibracao();
                break;

            case '2':
                dosar(250);
                break;

            case '3':
                dosar(300);
                break;

            case '4':
                dosar(350);
                break;

            default:
                printf("Opcao invalida\n");
                break;
        }
    }

    return 0;
}
