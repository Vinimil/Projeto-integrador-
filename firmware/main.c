#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/stdio_usb.h"
#include "pico/error.h"

#include "config.h"
#include "adc.h"


void bomba_setup(void) {
    gpio_init(BOMBA_PIN);
    gpio_set_dir(BOMBA_PIN, GPIO_OUT);
    gpio_put(BOMBA_PIN, 0); 
}

void bomba_ligar(void) {
    gpio_put(BOMBA_PIN, 1);
}

void bomba_desligar(void) {
    gpio_put(BOMBA_PIN, 0);
}


float ler_volume_atual(void) {
    uint16_t adc_raw = adc_read_raw();
    float percent = adc_to_percent(adc_raw);
    float altura = percent_to_height(percent);
    return height_to_volume(altura);
}


void dosar(float quantidade_ml)
{
    float volume_inicial = ler_volume_atual();
    float volume_alvo = volume_inicial - quantidade_ml; 

    printf("\n========================================================\n");
    printf("BOMBA LIGADA - DOSANDO %.0f mL\n", quantidade_ml);
    printf("========================================================\n");

    while(true)
    {
        float volume_atual = ler_volume_atual();
        float erro_atual = volume_atual - volume_alvo; 

        printf("\rAtual = %4.0f mL | Falta tirar = %4.0f mL \033[K", volume_atual, erro_atual);
        fflush(stdout);

       
        if(erro_atual <= COMPENSACAO_ML)
        {
            bomba_desligar(); 
            printf("\n  -> Alvo detectado! Aguardando a agua acalmar para confirmar...\n");
            
            sleep_ms(1200); 
            
            
            float volume_prova_real = ler_volume_atual();
            float erro_prova_real = volume_prova_real - volume_alvo;
            
            
            if (erro_prova_real <= (COMPENSACAO_ML + 10.0f)) {
                printf("[ CONFIRMADO ] Desligamento definitivo.\n");
                break; 
            } else {
                printf("[ ALARME FALSO ] Foi uma onda. Faltam %.0f mL. Retomando bomba...\n", erro_prova_real);
                continue; 
            }
        }

      
        if(erro_atual <= JANELA_APROXIMACAO_ML)
        {
            bomba_ligar();
            sleep_ms(PULSO_LIGADO_MS);
            bomba_desligar();
            sleep_ms(PULSO_DESLIGADO_MS);
        }
        else
        {
            bomba_ligar();
            sleep_ms(LOOP_DELAY_MS);
        }
    }

  
    printf("Calculando resumo final...\n");
    sleep_ms(1000); 
    
    float volume_final = ler_volume_atual();

    printf("\n========================================================\n");
    printf("                  RESUMO DA DOSAGEM                     \n");
    printf("========================================================\n");
    printf(" Nivel ANTES:  %4.0f mL\n", volume_inicial);
    printf(" Nivel DEPOIS: %4.0f mL\n", volume_final);
    printf(" Total Tirado: %4.0f mL\n", volume_inicial - volume_final);
    printf("========================================================\n\n");
}


int main(void) {
    stdio_init_all(); 

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    sleep_ms(1000); 

    adc_setup();
    bomba_setup();

    while (true) {
        
       

        printf("\033[2J\033[H"); 
        printf("========================================================\n");
        printf("               SISTEMA PRONTO E DESLIGADO               \n");
        printf("========================================================\n");
        printf(" Para dosar, digite o valor em mL e pressione [ENTER]\n");
        printf("========================================================\n\n");

        while (true) {
            float volume = ler_volume_atual();

            
            printf("\r  -> NIVEL DO TANQUE: %4.0f mL   |   > Digite aqui: \033[K", volume);
            fflush(stdout);

            int c = getchar_timeout_us(200000);

            
            if (c != PICO_ERROR_TIMEOUT) {
                ungetc(c, stdin); 
                break;            
            }
        }
        
        float dose_escolhida = 0.0f;
        
        
        if (scanf("%f", &dose_escolhida) == 1) {
            
            int lixo;
            while ((lixo = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT) { } 

            if (dose_escolhida >= DOSE_MIN_ML && dose_escolhida <= DOSE_MAX_ML) {
                
                dosar(dose_escolhida);
                
                while ((lixo = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT) { }
                printf("[ AVISO ] Pode retirar o recipiente.\n");
                printf("Pressione ENTER para fazer uma nova dosagem...\n");
                getchar(); 

            } else {
                printf("\n\n[ERRO] Valor invalido! Escolha um valor entre %d e %d.\n", DOSE_MIN_ML, DOSE_MAX_ML);
                sleep_ms(3000); 
            }
            
        } else {
            int lixo;
            while ((lixo = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT) { }
        }
    }

    return 0;
}
