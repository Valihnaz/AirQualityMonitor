#pragma once

#include <secrets.h>

// Hardware pins
#define DHT_PIN        15
#define MQ2_PIN        34
#define DHT_TYPE       DHT22
#define AUTO_LED       2
#define HEATER_PIN     26
#define HUMIDIFIER_PIN 27
#define FAN_PIN        25

// Blynk virtual pins
#define VPIN_TEMPERATURE 0
#define VPIN_HUMIDITY    1
#define VPIN_GAS         2
#define VPIN_CONNECT     4
#define VPIN_STATUS      5
#define VPIN_FAN         6
#define VPIN_HEATER      7
#define VPIN_HUMIDIFIER  8
#define VPIN_AUTO        9
#define VPIN_AVG_TEMP    10
#define VPIN_AVG_HUM     11
#define VPIN_AVG_GAS     12
#define VPIN_MAX_TEMP    13
#define VPIN_MIN_TEMP    14
#define VPIN_RESET       15

// Threshold values
#define TEMP_MIN      12.0f   // Minimum temperature in °C
#define TEMP_MAX      30.0f   // Maximum temperature in °C
#define HUM_MIN       40.0f   // Minimum humidity in %
#define HUM_MAX       60.0f   // Maximum humidity in %
#define GAS_THRESHOLD 450.0f  // LPG threshold in ppm

// MQ2 sensor calibration
#define MQ2_RL        10.0f    // Load resistor in kOhm
#define MQ2_R0        9.8f     // Sensor resistance in clean air
#define MQ2_A         574.25f  // LPG coefficient from datasheet
#define MQ2_B         -2.222f  // LPG exponent from datasheet

// History buffer size
#define HISTORY_SIZE 50