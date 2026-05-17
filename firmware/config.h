#ifndef CONFIG_H
#define CONFIG_H

// ADC

#define ADC_PIN_A          26
#define ADC_CHANNEL_A      0

#define ADC_PIN_B          27
#define ADC_CHANNEL_B      1


// CALIBRAÇÃO DOS TANQUES


#define ADC_MIN_A          500
#define ADC_MAX_A          60000

#define ADC_MIN_B          500
#define ADC_MAX_B          60000

//-------------------------------------
// FILTRO
//-------------------------------------

#define NUM_AMOSTRAS       5
#define HISTERESE          3.0f


// TEMPO


#define LOOP_DELAY_MS      200

//-------------------------------------
// REFERÊNCIA ADC
//-------------------------------------

#define ADC_VREF           3.3f
#define ADC_RESOLUTION     65535.0f


// TANQUE RETANGULAR


#define TANQUE_LARGURA_CM  34.5f
#define TANQUE_COMPR_CM    9.5f
#define TANQUE_ALTURA_CM   21.5f


// BOMBAS


#define BOMBA_A_PIN        14
#define BOMBA_B_PIN        15


// MISTURA (TESTE)


#define VOLUME_TOTAL_ML    1000.0f

#define PERCENTUAL_A       30.0f

#endif
