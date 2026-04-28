#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- Hardware Pins (Change these to match your board) ---
#define PIN_MOSFET_GATE 2
#define PIN_ADC_INPUT A0 // Pin 14
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

// --- Constants ---
const uint32_t PULSE_INTERVAL_US = 2000; // 500Hz Frequency
const uint32_t PULSE_WIDTH_US = 150;     // Time MOSFET is ON
const uint32_t SAMPLE_DELAY_US = 25;     // Wait for flyback to clear
const uint32_t SAMPLE_COUNT = 50;        // Number of samples taken after the delay

const uint32_t DETECTOR_CALIBRATION_LENGTH = 1000;  // The number of samples collected to change to detecting state
const float DETECTOR_LARGE_SIGNAL_THRESHOLD = 0.9f; // The signal stregnth threshold for a "Target"
const float DETECTOR_DECAY_THRESHOLD = 0.2f;        // The signal decay threshold for ground
const float DETECTOR_NOISE_FLOOR = 0.1f;            // The floor for signal stength

// Smoothing factors (0.0 to 1.0)
// alpha: High = responsive but noisy; Low = smooth but laggy
const float FILTER_SIGNAL_ALPHA = 0.2f;
const float FILTER_GROUND_ALPHA = 0.001f;

const uint32_t UI_REFRESH_RATE_MS = 50;

#endif