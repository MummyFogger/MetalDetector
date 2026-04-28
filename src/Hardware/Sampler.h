#ifndef SAMPLER_H
#define SAMPLER_H

#include <Arduino.h>
#include <ADC.h>

class Sampler
{
public:
    Sampler(uint8_t pin);

    // Initializes the ADC hardware
    void init();

    // Performs a high-speed synchronous read
    // marked 'inline' because this is called in the ISR Hot Path
    inline uint32_t read()
    {
        // analogRead blocks briefly, but at VERY_HIGH_SPEED
        // on a 600MHz chip, it is only a few microseconds.
        return _adc.adc0->analogRead(_pin);
    }

private:
    uint8_t _pin;
    ADC _adc; // Local instance for this class
};

#endif