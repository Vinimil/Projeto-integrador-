#ifndef CONFIG_H
#define CONFIG_H

// ---------- ADC ----------

#define ADC_PIN 26
#define ADC_CHANNEL 0

// ------ CALIBRACAO -------

#define ADC_MIN 400
#define ADC_MAX 3600

// -------- FILTRO ---------

#define NUM_AMOSTRAS 5

// -------- TEMPO ----------

#define TEMPO_LEITURA_MS 200

// ------- HISTERESE -------

#define HISTERESE_PERCENT 3.0f

// ------- ADC INFO --------

#define ADC_VREF 3.3f

#define ADC_RESOLUTION 4095.0f

#endif