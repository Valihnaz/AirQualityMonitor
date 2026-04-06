#include "blynk_handler.h"
#include "config.h"
#include "sensors.h"
#include "history.h"
#include "control.h"
#include "fsm.h"
#include <BlynkSimpleEsp32.h>

// Global variables & timers
bool isConnected = false;     // Connection flag for the Blynk service
static BlynkTimer timer;      // Timer instance for periodic tasks


//Sends sensor readings and calculated statistics to the Blynk app.
//Includes current values, averages, and min/max ranges.
void sendData() {
    // Check if sensor data is valid before sending
    if (isnan(temperature) || isnan(humidity)) {
        Blynk.virtualWrite(VPIN_STATUS, "DHT error");
        return;
    }

    // Push current sensor values
    Blynk.virtualWrite(VPIN_TEMPERATURE, temperature);
    Blynk.virtualWrite(VPIN_HUMIDITY,    humidity);
    Blynk.virtualWrite(VPIN_GAS,         gasPPM);

    // Push historical averages (0: Temp, 1: Hum, 2: Gas)
    Blynk.virtualWrite(VPIN_AVG_TEMP, getAverage(0));
    Blynk.virtualWrite(VPIN_AVG_HUM,  getAverage(1));
    Blynk.virtualWrite(VPIN_AVG_GAS,  getAverage(2));

    // Push temperature peaks
    Blynk.virtualWrite(VPIN_MIN_TEMP, getMin(0));
    Blynk.virtualWrite(VPIN_MAX_TEMP, getMax(0));

    // Update general system status message
    Blynk.virtualWrite(VPIN_STATUS, status);
}


//Synchronizes the physical actuator states (relays/mosfets) and the current FSM mode back to the Blynk interface.

void syncActuators() {
    Blynk.virtualWrite(VPIN_HEATER,     heaterState);
    Blynk.virtualWrite(VPIN_HUMIDIFIER, humidifierState);
    Blynk.virtualWrite(VPIN_FAN,        fanState);
    Blynk.virtualWrite(VPIN_AUTO,       currentState == STATE_AUTO);
}


//Initializes the Blynk connection and sets up periodic timers.

void blynkInit(const char* auth, const char* ssid, const char* pass) {
    Blynk.begin(auth, ssid, pass);
    
    // Core logic cycle interval
    timer.setInterval(500L, mainCycle);
    
    // Data reporting interval (2 seconds)
    timer.setInterval(2000L, sendData); 
}

void blynkRun() {
    Blynk.run();
}

void blynkTimerRun() {
    timer.run();
}


//The primary logic controller. Prevents FSM execution unless the system is explicitly "connected".
void mainCycle() {
    if (!isConnected) return;

    fsmRun();
}

void blynkWriteStatus(const String& msg) {
    Blynk.virtualWrite(VPIN_STATUS, msg);
}

//Blynk Input Handlers (Virtual Pins)

BLYNK_WRITE(VPIN_CONNECT) {
    bool on = param.asInt();
    if (on) {
        // Default to AUTO mode upon system activation
        fsmTransition(STATE_AUTO);
    } else {
        fsmTransition(STATE_OFF);
    }
    isConnected = on;
}

BLYNK_WRITE(VPIN_AUTO) {
    // Ignore input if the system is turned OFF
    if (currentState == STATE_OFF) return;

    if (param.asInt()) {
        fsmTransition(STATE_AUTO);
    } else {
        fsmTransition(STATE_MANUAL);
    }
}


//Manual Overrides
//These only function when the system is in STATE_MANUAL
BLYNK_WRITE(VPIN_HEATER) {
    if (currentState != STATE_MANUAL) return;
    heaterState = param.asInt();
}

BLYNK_WRITE(VPIN_HUMIDIFIER) {
    if (currentState != STATE_MANUAL) return;
    humidifierState = param.asInt();
}

BLYNK_WRITE(VPIN_FAN) {
    if (currentState != STATE_MANUAL) return;
    fanState = param.asInt();
}


//System Maintenance
//Clears historical data and resets actuator control states.

BLYNK_WRITE(VPIN_RESET) {
    if (param.asInt() == 1) {
        resetHistory();
        resetControl();
        blynkWriteStatus("System reset");
    }
}