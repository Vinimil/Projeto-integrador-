#include "calibracao.h"

#define NUM_PONTOS 16

static const uint16_t adc_pontos[NUM_PONTOS] =
{
    3057,
    3054,
    3041,
    2909,
    2584,
    2389,
    2109,
    1907,
    1672,
    1503,
    1314,
    1067,
    843,
    632,
    531,
    529
};

static const float volume_pontos[NUM_PONTOS] =
{
       0.0f,
     200.0f,
     400.0f,
     600.0f,
     800.0f,
    1000.0f,
    1200.0f,
    1400.0f,
    1600.0f,
    1800.0f,
    2000.0f,
    2200.0f,
    2400.0f,
    2600.0f,
    2800.0f,
    3000.0f
};

float adc_para_volume(uint16_t adc)
{
    if(adc >= adc_pontos[0])
        return volume_pontos[0];

    if(adc <= adc_pontos[NUM_PONTOS - 1])
        return volume_pontos[NUM_PONTOS - 1];

    for(int i = 0; i < NUM_PONTOS - 1; i++)
    {
        if(adc <= adc_pontos[i] &&
           adc >= adc_pontos[i + 1])
        {
            float adc1 = adc_pontos[i];
            float adc2 = adc_pontos[i + 1];

            float vol1 = volume_pontos[i];
            float vol2 = volume_pontos[i + 1];

            float t =
                (adc - adc1) /
                (adc2 - adc1);

            return vol1 +
                   t *
                   (vol2 - vol1);
        }
    }

    return 0.0f;
}
