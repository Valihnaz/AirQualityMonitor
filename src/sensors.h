#pragma once
#include <Arduino.h>

// Global sensor values
extern float temperature; // Current temperature in Celsius
extern float humidity;    // Current humidity in %
extern float gasPPM;      // Current gas concentration (ppm)
extern String status;     // System status message

// Sensor Functions
void sensorsInit();   // Initialize DHT and MQ2 sensors
void readSensors();   // Read current values from sensors