#pragma once
#include "fsm.h"
// Control state flags
extern bool autoMode;
extern bool fanState;
extern bool heaterState;
extern bool humidifierState;

// Initialize actuator pins
void controlInit();

// Apply control logic to actuators based on sensor readings
void applyControl(SystemState_t state);

// Reset all control states and outputs to default
void resetControl();

// Update system status string based on sensor readings
void updateStatus();