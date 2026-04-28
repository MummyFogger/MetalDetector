#include "Filter.h"

Filter::Filter() : _currentEstimate(0.0f), _groundLevel(0.0f) {}

float Filter::pushSample(uint32_t rawValue)
{
    /* Exponential Moving Average Formula:
       Output = (Alpha * NewSample) + ((1 - Alpha) * PreviousOutput)
    */
    _currentEstimate = (_signalAlpha * (float)rawValue) +
                       ((1.0f - _signalAlpha) * _currentEstimate);

    return _currentEstimate;
}

void Filter::updateGroundReference(float cleanValue)
{
    // This tracks the "Baseline." We use a very tiny Alpha so that
    // a quick metal target doesn't change the ground level,
    // but moving from clay to sand slowly will.
    if (_groundLevel == 0.0f)
        _groundLevel = cleanValue; // Initial seed

    _groundLevel = (_groundAlpha * cleanValue) +
                   ((1.0f - _groundAlpha) * _groundLevel);
}

float Filter::getDelta(float cleanValue) const
{
    // This is what the UI actually displays.
    // If the value is > 0, metal is likely present.
    float delta = cleanValue - _groundLevel;
    return (delta > 0) ? delta : 0; // Ignore negative swings
}