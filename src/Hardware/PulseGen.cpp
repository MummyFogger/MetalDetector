#include "PulseGen.h"

void PulseGen::init()
{
    pinMode(_gatePin, OUTPUT);
    digitalWriteFast(_gatePin, LOW);
}