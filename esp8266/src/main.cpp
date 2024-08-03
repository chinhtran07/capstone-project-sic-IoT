#include <Arduino.h>
#include "WiFiManagerWrapper.h"
#include "MQTTManager.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
// #include "ThingSpeakManager.h"
// #include "FirebaseManager.h"
#include "TimeManager.h"
// #include "SensorManager.h"
WiFiManagerWrapper wifiManager;
MQTTManager awsManager;
// ThingSpeakManager thingSpeakManager;
// FirebaseManager firebaseManager;
TimeManager timeManager;
// SensorManager sensorManager;
SoftwareSerial sensor(D1, D2);

bool readDataFromUART(float &humidity, float &temperature, int &soilMoisture);

void setup() {
    Serial.begin(115200);
    sensor.begin(9600);
    wifiManager.setup();
    timeManager.setup();
    awsManager.setup();
    // thingSpeakManager.setup();
    // firebaseManager.setup();
    // sensorManager.setup();
}

void loop() {
    float humidity, temperature;
    int soilMoisture;
    if (readDataFromUART(humidity, temperature, soilMoisture)) {
        JsonDocument doc;
        doc["time"] = millis();
        doc["humidity"] = humidity;
        doc["temperature"] = temperature;
        doc["soil moisture"] = soilMoisture;

        awsManager.publishMessage(doc.as<JsonObject>());
    }

    awsManager.loop();

    delay(1000); // Delay to prevent overloading services
}

bool readDataFromUART(float &humidity, float &temperature, int &soilMoisture) {
    if (sensor.available()) {
        JsonDocument doc;
        String data = sensor.readStringUntil('\n');
        DeserializationError error = deserializeJson(doc, data);
        if (!error) {
            humidity = doc["humidity"];
            temperature = doc["temperature"];
            soilMoisture = doc["soil moisture"];
        }

        return true;
    }
    return false;
}