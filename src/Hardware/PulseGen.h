#ifndef PULSE_GEN_H
#define PULSE_GEN_H

#include <Arduino.h>

class PulseGen
{
public:
    PulseGen(uint8_t pin) : _gatePin(pin) {}

    // Method defined in .cpp (Not time-critical)
    void init();

    // Method defined in .h (Time-critical!)
    // Inline allows the compiler to optimize out the function call overhead.
    inline void fire(uint32_t widthMicros)
    {
        digitalWriteFast(_gatePin, HIGH);
        delayMicroseconds(widthMicros);
        digitalWriteFast(_gatePin, LOW);
    }

private:
    uint8_t _gatePin;
};

#endif