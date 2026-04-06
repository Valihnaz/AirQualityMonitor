#include "fsm.h"
#include "config.h"
#include "sensors.h"
#include "history.h"
#include "control.h"
#include "blynk_handler.h"
#include <Arduino.h>

SystemState_t currentState = STATE_OFF;

// Actions when entering a state
static void onEnter(SystemState_t state) {
    switch (state) {

        case STATE_OFF:
            resetControl();
            syncActuators();
            blynkWriteStatus("System OFF");
            Serial.println("STATE_OFF");
            break;

        case STATE_MANUAL:
            autoMode = false;
            digitalWrite(AUTO_LED, LOW);
            blynkWriteStatus("Manual control");
            break;

        case STATE_AUTO:
            autoMode = true;
            digitalWrite(AUTO_LED, HIGH);
            blynkWriteStatus("Auto control");
            break;
    }
}

// Actions when exiting a state
static void onExit(SystemState_t state) {
    switch (state) {
        case STATE_OFF:
            break;

        case STATE_MANUAL:
        case STATE_AUTO:
            // Ensure a safe state on every transition
            resetControl();
            break;
    }
}

void fsmTransition(SystemState_t newState) {
    if (currentState == newState) return;

    onExit(currentState);
    currentState = newState;
    onEnter(newState);
}

// State logic execution — called every cycle
void fsmRun() {
    switch (currentState) {

        case STATE_OFF:
            // Do nothing
            break;

        case STATE_MANUAL:
            readSensors();
            updateHistory(temperature, humidity, gasPPM);
            updateStatus();
            applyControl(currentState);
            syncActuators();
            break;

        case STATE_AUTO:
            readSensors();
            updateHistory(temperature, humidity, gasPPM);
            updateStatus();
            applyControl(currentState);
            syncActuators();
            break;
    }
}