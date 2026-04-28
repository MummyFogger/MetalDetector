#include <Arduino.h>
#include <ADC.h>
#include <ILI9341_t3.h>
#include <SPI.h>
#include "Config.h"
#include "Hardware/PulseGen.h"
#include "Hardware/Sampler.h"
#include "DSP/Filter.h"
#include "Detector.h"

// --- Global Objects (Static Allocation) ---
PulseGen coil(PIN_MOSFET_GATE);
Sampler sampler(PIN_ADC_INPUT);
Filter filter;
Detector detector(filter);
ADC adc;
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST);
IntervalTimer pulseTimer;

// --- Volatile Shared State ---
// "volatile" tells the compiler: "This changes inside an interrupt!"
volatile bool dataReady = false;

void drivePulse()
{
    static uint32_t burstBuffer[SAMPLE_COUNT];

    // 1. Fire the coil
    coil.fire(PULSE_WIDTH_US);

    // 2. Wait for Flyback/Zener clamping to stabilize
    delayMicroseconds(SAMPLE_DELAY_US);

    for (uint32_t i = 0; i < SAMPLE_COUNT; i++)
    {
        // 3. Sample the Decay
        // TODO possible upgrade sampler.read() to asynchronous version
        burstBuffer[i] = sampler.read();
    }

    // 4. Update the detector state machine with the new raw data
    detector.processBurst(burstBuffer, SAMPLE_COUNT);
    dataReady = true;
}

// --- 2. Initialization ---
void setup()
{
    Serial.begin(115200);

    // GPIO Setup
    coil.init();

    // ADC Configuration
    sampler.init();

    // TFT Setup
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("PI Detector Init...");

    // Start the High-Priority Timer
    pulseTimer.begin(drivePulse, PULSE_INTERVAL_US);
}

// --- 3. Background Loop (UI & Logging) ---
void loop()
{
    if (dataReady)
    {
        // Reset flag immediately
        dataReady = false;

        // Visualizing in Serial Plotter (VS Code PlatformIO handles this)
        Serial.println("Detector Data:");
        Serial.print("Raw Average Signal ");
        Serial.println(detector.getAverageSignal());
        Serial.print("Clean Average Signal ");
        Serial.println(detector.getCleanValue());
        Serial.print("Target Strength ");
        Serial.println(detector.getTargetStrength());
        Serial.print("Decay Ratio ");
        Serial.println(detector.getDecayRatio());

        // Update UI every ~50ms (Don't over-refresh!)
        static uint32_t lastUIUpdate = 0;
        if (millis() - lastUIUpdate > UI_REFRESH_RATE_MS)
        {
            tft.fillRect(100, 50, 100, 30, ILI9341_BLACK); // Clear old text
            tft.setCursor(100, 50);
            tft.print(detector.getTargetStrength());
            lastUIUpdate = millis();
        }
    }
}