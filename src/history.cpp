#include "history.h"
#include "config.h"

// Circular buffer to store sensor readings
static SensorData history[HISTORY_SIZE];
static int historyIndex = 0;  // Current position in the buffer
int dataCount = 0;             // Number of valid entries in history

// Add a new set of sensor readings to history
void updateHistory(float t, float h, float gas) {
    history[historyIndex] = {t, h, gas};
    historyIndex = (historyIndex + 1) % HISTORY_SIZE; // wrap around
    if (dataCount < HISTORY_SIZE) dataCount++;
}

// Reset the history buffer
void resetHistory() {
    historyIndex = 0;
    dataCount    = 0;
}

// Internal helper to get a value by type: 0=temperature, 1=humidity, 2=gas
static float getValue(int i, int type) {
    if (type == 0) return history[i].temp;
    if (type == 1) return history[i].hum;
    return history[i].gas;
}

// Calculate the average of a specific type over stored history
float getAverage(int type) {
    if (dataCount == 0) return 0;
    float sum = 0;
    for (int i = 0; i < dataCount; i++) sum += getValue(i, type);
    return sum / dataCount;
}

// Get the minimum value of a specific type from history
float getMin(int type) {
    if (dataCount == 0) return 0;
    float minVal = 999999;
    for (int i = 0; i < dataCount; i++) {
        float v = getValue(i, type);
        if (v < minVal) minVal = v;
    }
    return minVal;
}

// Get the maximum value of a specific type from history
float getMax(int type) {
    if (dataCount == 0) return 0;
    float maxVal = -999999;
    for (int i = 0; i < dataCount; i++) {
        float v = getValue(i, type);
        if (v > maxVal) maxVal = v;
    }
    return maxVal;
}