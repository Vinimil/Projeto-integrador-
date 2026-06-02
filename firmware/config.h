#ifndef CONFIG_H
#define CONFIG_H

// ADC
#define ADC_PIN          26
#define ADC_CHANNEL      0

#define ADC_MIN          500
#define ADC_MAX          60000

// FILTRO
#define NUM_AMOSTRAS     5

// LOOP
#define LOOP_DELAY_MS    100

// TANQUE (cm)
#define TANQUE_COMPR_CM    33.7f
#define TANQUE_LARGURA_CM  17.0f
#define TANQUE_ALTURA_CM   12.8f

// DOSAGEM
#define DOSE_MIN_ML   100
#define DOSE_MAX_ML   3000
#define DOSE_STEP     50

#endif
