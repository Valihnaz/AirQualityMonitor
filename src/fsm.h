#pragma once

typedef enum {
    STATE_OFF,
    STATE_MANUAL,
    STATE_AUTO
} SystemState_t;

extern SystemState_t currentState;

void fsmTransition(SystemState_t newState);
void fsmRun();