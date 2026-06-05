#ifndef CONFIG_H
#define CONFIG_H


#define ADC_PIN                26
#define ADC_CHANNEL            0

#define ADC_MIN                1315
#define ADC_MAX                3957


#define NUM_AMOSTRAS           200 


#define TANQUE_COMPR_CM        30.0f
#define TANQUE_LARGURA_CM      15.0f
#define TANQUE_ALTURA_CM       9.0f


#define BOMBA_PIN              15
#define LOOP_DELAY_MS          100

#define DOSE_MIN_ML            100
#define DOSE_MAX_ML            3000
#define DOSE_STEP_ML           50


#define COMPENSACAO_ML         25.0f

#define JANELA_APROXIMACAO_ML  100.0f
#define PULSO_LIGADO_MS        200 
#define PULSO_DESLIGADO_MS     100

#define FATOR_CALIBRACAO       1.0f 

#endif
