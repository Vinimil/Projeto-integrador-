#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "config.h"
#include "adc.h"

void bomba_init(void)
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

float ler_volume_atual(void)
{
    uint16_t raw =
        adc_read_raw();

    float nivel =
        adc_to_percent(raw);

    float altura =
        percent_to_height(nivel);

    return
        height_to_volume(altura);
}

void mostrar_menu(
    float nivel,
    float volume,
    int dose)
{
    printf("\n");
    printf("====================================\n");
    printf("      DOSADOR AUTOMATICO V1.0B\n");
    printf("====================================\n");

    printf("Nivel tanque : %.1f %%\n", nivel);
    printf("Volume       : %.0f mL\n", volume);
    printf("Dose atual   : %d mL\n", dose);

    printf("\n");
    printf("[+] aumentar dose\n");
    printf("[-] diminuir dose\n");
    printf("[s] iniciar dosagem\n");
    printf("[q] dose padrao\n");
    printf("\n> ");
}

void dosar(int dose_ml)
{
    float volume_inicial =
        ler_volume_atual();

    if(volume_inicial < dose_ml)
    {
        printf("\n");
        printf("ERRO: volume insuficiente\n");
        printf("\n");
        return;
    }

    float alvo =
        volume_inicial -
        dose_ml +
        COMPENSACAO_ML;

    printf("\n");
    printf("Volume inicial : %.0f mL\n", volume_inicial);
    printf("Dose solicitada: %d mL\n", dose_ml);
    printf("Volume alvo    : %.0f mL\n", alvo);
    printf("\n");

    bomba_ligar();

    while(true)
    {
        float volume_atual =
            ler_volume_atual();

        float retirado =
            volume_inicial -
            volume_atual;

        float restante =
            dose_ml -
            retirado;

        printf(
            "\rRetirado: %.0f / %d mL",
            retirado,
            dose_ml
        );

        fflush(stdout);

        if(restante <= JANELA_APROXIMACAO_ML)
            break;

        sleep_ms(50);
    }

    while(true)
    {
        float volume_atual =
            ler_volume_atual();

        if(volume_atual <= alvo)
            break;

        bomba_ligar();
        sleep_ms(PULSO_LIGADO_MS);

        bomba_desligar();
        sleep_ms(PULSO_DESLIGADO_MS);

        float retirado =
            volume_inicial -
            volume_atual;

        printf(
            "\rAjuste fino: %.0f / %d mL",
            retirado,
            dose_ml
        );

        fflush(stdout);
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

    sleep_ms(2000);

    int dose = 700;

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

        mostrar_menu(
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
            dosar(dose);
        }

        if(c == 'q')
        {
            dose = 700;
        }

        sleep_ms(100);
    }

    return 0;
}
