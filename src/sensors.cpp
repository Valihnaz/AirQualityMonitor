#include "sensors.h"
#include "config.h"
#include <DHT.h>

// Global sensor values
float temperature = 0;
float humidity    = 0;
float gasPPM      = 0;
String status     = "";

// DHT sensor instance
static DHT dht(DHT_PIN, DHT_TYPE);

// MQ2 Gas Sensor Reading
static float readMQ2() {
    int raw = analogRead(MQ2_PIN); // Read raw ADC value
    float voltage = (raw / 4095.0f) * 3.3f; // Convert to voltage (ESP32 12-bit ADC)

    // Prevent division by zero
    if (voltage <= 0.01f) return 0;

    // Calculate sensor resistance
    float RS = ((3.3f - voltage) / voltage) * MQ2_RL;
    float ratio = RS / MQ2_R0;

    // Convert to ppm using datasheet formula: ppm = A * (RS/R0)^B
    float ppm = MQ2_A * pow(ratio, MQ2_B);

    // Constrain to realistic range
    return constrain(ppm, 0, 10000);
}

// Initialize Sensors
void sensorsInit() {
    dht.begin();                     // Start DHT sensor
    analogReadResolution(12);        // ESP32 12-bit ADC
    analogSetAttenuation(ADC_11db); // Full voltage range
    pinMode(MQ2_PIN, INPUT);         // MQ2 analog pin
}

// Read All Sensors
void readSensors() {
    temperature = dht.readTemperature();
    humidity    = dht.readHumidity();
    gasPPM      = readMQ2();
}