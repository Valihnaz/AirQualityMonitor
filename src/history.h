#pragma once

// Structure to store a single sensor reading
struct SensorData {
    float temp;  // Temperature in Celsius
    float hum;   // Humidity in %
    float gas;   // Gas concentration in ppm
};

// Add a new reading to history
void updateHistory(float t, float h, float gas);

// Get average value of a specific type over stored history
// type: 0 = temperature, 1 = humidity, 2 = gas
float getAverage(int type);

// Get minimum value of a specific type from history
float getMin(int type);

// Get maximum value of a specific type from history
float getMax(int type);

// Reset the history buffer
void resetHistory();

// Number of valid entries currently stored in history
extern int dataCount;