/src
├── main.cpp // Entry point, setup(), and main loop()
├── Config.h // Physical constants (Pin numbers, Sample rates)
├── Detector.h/cpp // Main logic class (The "Orchestrator")
├── Hardware/
│ ├── PulseGen.h // MOSFET driver logic
│ └── Sampler.h // ADC & DMA configuration
├── DSP/
│ ├── Filters.h // Moving averages, High-pass filters
│ └── GroundComp.h // Ground tracking algorithms
└── UI/
└── Display.h // ILI9341 wrapper
