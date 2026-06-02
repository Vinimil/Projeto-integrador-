#ifndef CONFIG_H
#define CONFIG_H

// ADC

#define ADC_PIN                26
#define ADC_CHANNEL            0

#define ADC_MIN                500
#define ADC_MAX                3500

#define NUM_AMOSTRAS           10

// TANQUE

#define TANQUE_COMPR_CM        33.7f
#define TANQUE_LARGURA_CM      17.0f
#define TANQUE_ALTURA_CM       12.8f

// LOOP

#define LOOP_DELAY_MS          100

// DOSAGEM

#define DOSE_MIN_ML            100
#define DOSE_MAX_ML            3000
#define DOSE_STEP_ML           50

// BOMBA

#define BOMBA_PIN              15

// compensação

#define COMPENSACAO_ML         25.0f

// aproximação

#define JANELA_APROXIMACAO_ML  100.0f

#define PULSO_LIGADO_MS        80
#define PULSO_DESLIGADO_MS     120

#endif
