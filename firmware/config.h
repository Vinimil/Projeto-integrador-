#ifndef CONFIG_H
#define CONFIG_H


// ADC


#define ADC_PIN 26
#define ADC_CHANNEL 0

#define ADC_MIN 500
#define ADC_MAX 3500


// FILTRO


#define NUM_AMOSTRAS 5

#define HISTERESE 3.0f


// TANQUE


#define TANQUE_COMPRIMENTO_CM 34.5f

#define TANQUE_LARGURA_CM 9.5f

#define TANQUE_ALTURA_CM 21.5f


// MISTURA


#define VOLUME_TOTAL_ML 2000.0f

// usuário
#define PERCENTUAL_A 40.0f

//
// BOMBAS
// 
#define BOMBA_A_PIN 15
#define BOMBA_B_PIN 16

#endif
