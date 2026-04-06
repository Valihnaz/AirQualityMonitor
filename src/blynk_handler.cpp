#include "blynk_handler.h"
#include "config.h"
#include "sensors.h"
#include "history.h"
#include "control.h"
#include <BlynkSimpleEsp32.h>

// Flag for Blynk connection
bool isConnected = false;

// Timer for periodic data sending
static BlynkTimer timer; 

// Send sensor data and statistics to Blynk
void sendData() {
    if (isnan(temperature) || isnan(humidity)) {
        // If sensor data is invalid
        Blynk.virtualWrite(VPIN_STATUS, "DHT error");
        return;
    }

    // Current sensor values
    Blynk.virtualWrite(VPIN_TEMPERATURE, temperature);
    Blynk.virtualWrite(VPIN_HUMIDITY,    humidity);
    Blynk.virtualWrite(VPIN_GAS,         gasPPM);

    // Average values
    Blynk.virtualWrite(VPIN_AVG_TEMP, getAverage(0));
    Blynk.virtualWrite(VPIN_AVG_HUM,  getAverage(1));
    Blynk.virtualWrite(VPIN_AVG_GAS,  getAverage(2));

    // Min and max values
    Blynk.virtualWrite(VPIN_MIN_TEMP, getMin(0));
    Blynk.virtualWrite(VPIN_MAX_TEMP, getMax(0));

    // System status
    Blynk.virtualWrite(VPIN_STATUS, status);
}

// Sync actuator states to Blynk
void syncActuators() {
    Blynk.virtualWrite(VPIN_HEATER,     heaterState);
    Blynk.virtualWrite(VPIN_HUMIDIFIER, humidifierState);
    Blynk.virtualWrite(VPIN_FAN,        fanState);
    Blynk.virtualWrite(VPIN_AUTO,       autoMode);
}

// Initialize Blynk
void blynkInit(const char* auth, const char* ssid, const char* pass) {
    Blynk.begin(auth, ssid, pass);
    timer.setInterval(2000L, sendData); // send data every 2 seconds
}

// Run Blynk in the main loop
void blynkRun() {
    Blynk.run();
}

// Run timer tasks
void blynkTimerRun() {
    timer.run();
}

// Handle Blynk virtual pins
BLYNK_WRITE(VPIN_CONNECT)    { isConnected     = param.asInt(); }
BLYNK_WRITE(VPIN_AUTO)       { autoMode        = param.asInt(); }
BLYNK_WRITE(VPIN_HEATER)     { heaterState     = param.asInt(); }
BLYNK_WRITE(VPIN_HUMIDIFIER) { humidifierState = param.asInt(); }
BLYNK_WRITE(VPIN_FAN)        { fanState        = param.asInt(); }

// Reset history and control states
BLYNK_WRITE(VPIN_RESET) {
    if (param.asInt() == 1) {
        resetHistory();
        resetControl();
        Blynk.virtualWrite(VPIN_STATUS, "System reset");
    }
}