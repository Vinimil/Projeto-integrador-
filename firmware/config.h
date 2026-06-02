#ifndef CONFIG_H
#define CONFIG_H

// ADC

#define ADC_PIN              26
#define ADC_CHANNEL          0

// Ajustar
#define ADC_MIN              0
#define ADC_MAX              4095

#define NUM_AMOSTRAS         5


// TANQUE


#define TANQUE_COMPR_CM      33.7f
#define TANQUE_LARGURA_CM    17.0f
#define TANQUE_ALTURA_CM     12.8f


// LOOP


#define LOOP_DELAY_MS        100

// DOSAGEM

#define DOSE_MIN_ML          100
#define DOSE_MAX_ML          3000
#define DOSE_STEP_ML         50

// BOMBA

#define BOMBA_PIN            15

// overshoot
#define COMPENSACAO_ML       25.0f

// Valor inicial para testes
#define VAZAO_BOMBA_ML_S     100.0f

#define TEMPO_MIN_BOMBA_MS   500

#endif
