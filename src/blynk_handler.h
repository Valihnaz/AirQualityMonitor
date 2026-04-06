#pragma once

// Blynk connection status flag
extern bool isConnected;

// Initialize Blynk with auth token and Wi-Fi credentials
void blynkInit(const char* auth, const char* ssid, const char* pass);

// Run Blynk in main loop
void blynkRun();

// Run timer tasks (periodic functions)
void blynkTimerRun();

// Send sensor data and statistics to Blynk
void sendData();

// Sync actuator states (heater, fan, humidifier) to Blynk
void syncActuators();