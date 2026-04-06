#define BLYNK_PRINT Serial

#include "config.h"
#include "sensors.h"
#include "history.h"
#include "control.h"
#include "blynk_handler.h"
#include <WiFi.h>

void setup() {
    // Initialize serial output
    Serial.begin(115200);

    // Initialize sensors, actuators, and Blynk
    sensorsInit();
    controlInit();
    blynkInit(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

    Serial.println("System started");
}

void loop() {
    // Run Blynk client and timer
    blynkRun();
    blynkTimerRun();
}


