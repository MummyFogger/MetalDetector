#include "Detector.h"

Detector::Detector(Filter &dsp)
    : _dsp(dsp)
{
    // Member initializer list used above is the C++ way to handle references
}

void Detector::init()
{
    _currentState = DetectorState::CALIBRATING;
}

void Detector::processBurst(uint32_t *samples, uint16_t count)
{
    float sum = 0;
    for (uint16_t i = 0; i < count; i++)
    {
        sum += samples[i];
    }
    _averageSignal = sum / count;
    _cleanValue = _dsp.pushSample(_averageSignal);

    // TODO make decay ratio more accurate
    float early = samples[0] - DETECTOR_NOISE_FLOOR;
    float late = samples[count - 1] - DETECTOR_NOISE_FLOOR;
    if (early < 0)
        early = 0;
    if (late < 0)
        late = 0;
    float decayRatio = (early > 0) ? (late / early) : 0;

    bool isNotMassiveTarget = (_cleanValue < DETECTOR_LARGE_SIGNAL_THRESHOLD);
    bool isGround = (decayRatio < DETECTOR_DECAY_THRESHOLD);

    switch (_currentState)
    {
    case DetectorState::CALIBRATING:
        if (isGround && isNotMassiveTarget)
        {
            _dsp.updateGroundReference(_cleanValue);
            _sampleCount++;
            if (_sampleCount > DETECTOR_CALIBRATION_LENGTH)
            { // After 1000 samples, start detecting
                _currentState = DetectorState::DETECTING;
            }
        }
        break;

    case DetectorState::DETECTING:
        // Calculate how much the signal differs from the ground
        _targetStrength = _dsp.getDelta(_cleanValue);
        _decayRatio = decayRatio;
        break;

    default:
        break;
    }
}

void Detector::update()
{
    // This runs in the main loop()
    // Perform "slow" tasks here, like checking for hardware errors
    // or processing buttons/user input.
}