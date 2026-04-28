#ifndef FILTER_H
#define FILTER_H

#include <Arduino.h>
#include "Config.h"

class Filter
{
public:
    Filter();

    // The Hot Path: Smooths the raw ADC jitter
    float pushSample(uint32_t rawValue);

    // The Ground Path: Slowly tracks the soil's base signal
    void updateGroundReference(float cleanValue);

    // Calculation: Signal minus Ground
    float getDelta(float cleanValue) const;

    // Getters for calibration
    float getGroundLevel() const { return _groundLevel; }

private:
    float _currentEstimate = 0.0f;
    float _groundLevel = 0.0f;

    // Smoothing factors (0.0 to 1.0)
    // alpha: High = responsive but noisy; Low = smooth but laggy
    const float _signalAlpha = FILTER_SIGNAL_ALPHA;
    const float _groundAlpha = FILTER_GROUND_ALPHA; // Very slow tracking
};

#endif