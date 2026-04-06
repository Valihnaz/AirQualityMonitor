#include "control.h"
#include "config.h"
#include "sensors.h"
#include <Arduino.h>

// Control states
bool autoMode        = true;
bool fanState        = false;
bool heaterState     = false;
bool humidifierState = false;

// Initialize output pins
void controlInit() {
    pinMode(FAN_PIN,        OUTPUT);
    pinMode(HEATER_PIN,     OUTPUT);
    pinMode(HUMIDIFIER_PIN, OUTPUT);
    pinMode(AUTO_LED,       OUTPUT);
}

// Reset control states to default
void resetControl() {
    autoMode = true;
    fanState = heaterState = humidifierState = false;

    digitalWrite(HEATER_PIN,     LOW);
    digitalWrite(HUMIDIFIER_PIN, LOW);
    digitalWrite(FAN_PIN,        LOW);
    digitalWrite(AUTO_LED,       LOW);
}

// Update the status string based on sensor readings
void updateStatus() {
    bool tempOk = (temperature >= TEMP_MIN && temperature <= TEMP_MAX);
    bool humOk  = (humidity >= HUM_MIN && humidity <= HUM_MAX);
    bool gasOk  = (gasPPM < GAS_THRESHOLD);

    if (tempOk && humOk && gasOk) {
        status = "Comfort conditions";
    } else {
        status = "Adjustment needed: ";
        if (!tempOk) status += "TEMP ";
        if (!humOk)  status += "HUM ";
        if (!gasOk)  status += "GAS ";
    }
}

// Apply control actions to actuators
void applyControl() {
    if (autoMode) {
        digitalWrite(AUTO_LED, HIGH);

        // Heater control
        if (!heaterState && temperature < TEMP_MIN) heaterState = true;
        if (heaterState  && temperature > TEMP_MAX) heaterState = false;

        // Humidifier control
        if (!humidifierState && humidity < HUM_MIN) humidifierState = true;
        if (humidifierState  && humidity > HUM_MAX) humidifierState = false;

        // Fan control
        if (!fanState && gasPPM > GAS_THRESHOLD) fanState = true;
        if (fanState  && gasPPM < GAS_THRESHOLD) fanState = false;

    } else {
        digitalWrite(AUTO_LED, LOW); // Manual mode
    }

    // Apply output states to pins
    digitalWrite(HEATER_PIN,     heaterState);
    digitalWrite(HUMIDIFIER_PIN, humidifierState);
    digitalWrite(FAN_PIN,        fanState);
}