#include "Sampler.h"

Sampler::Sampler(uint8_t pin) : _pin(pin)
{
    // Member initialization
}

void Sampler::init()
{
    pinMode(_pin, INPUT_DISABLE);

    // Configure ADC0 (Teensy 4.1 has two ADCs, we use the first)
    _adc.adc0->setResolution(12); // 12-bit is the sweet spot for speed

    // Set conversion speed to reach ~1 MSPS
    _adc.adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED);
    _adc.adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);

    // Hardware Averaging: 4 samples are taken and averaged by the hardware
    // before the 'read()' returns. This reduces noise at the cost of slight latency.
    _adc.adc0->setAveraging(4);

    Serial.println("Sampler: ADC initialized at High Speed.");
}