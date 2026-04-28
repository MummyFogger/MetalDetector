#ifndef DETECTOR_H
#define DETECTOR_H

#include <Arduino.h>
#include "DSP/Filter.h"
#include "Config.h"

// Define the operational states
enum class DetectorState
{
    BOOTING,
    CALIBRATING, // Ground balancing
    DETECTING,
    ERROR
};

class Detector
{
public:
    // Only needs the DSP engine to do its job
    Detector(Filter &dsp);

    void init();

    void processBurst(uint32_t *samples, uint16_t count);

    inline DetectorState getDetectorState() const
    {
        DetectorState snapshot;
        noInterrupts();
        snapshot = _currentState;
        interrupts();
        return snapshot;
    }
    inline float getAverageSignal() const
    {
        float snapshot;
        noInterrupts();
        snapshot = _averageSignal;
        interrupts();
        return snapshot;
    }
    inline float getCleanValue() const
    {
        float snapshot;
        noInterrupts();
        snapshot = _cleanValue;
        interrupts();
        return snapshot;
    }
    inline float getTargetStrength() const
    {
        float snapshot;
        noInterrupts();
        snapshot = _targetStrength;
        interrupts();
        return snapshot;
    }
    inline float getDecayRatio() const
    {
        float snapshot;
        noInterrupts();
        snapshot = _decayRatio;
        interrupts();
        return snapshot;
    }

    void update();

private:
    Filter &_dsp;
    volatile uint32_t _sampleCount = 0;
    volatile float _averageSignal = 0.0f;
    volatile float _cleanValue = 0.0f;
    volatile float _targetStrength = 0.0f;
    volatile float _decayRatio = 0.0f;
    volatile DetectorState _currentState = DetectorState::BOOTING;
};

#endif