#include <Arduino.h>
#include "WiFiManagerWrapper.h"
#include "MQTTManager.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "TimeManager.h"


WiFiManagerWrapper wifiManager;
TimeManager timeManager;
SoftwareSerial sensor(D1, D2);
MQTTManager awsManager(sensor);

float humidity, temperature;
int soilMoisture;

bool readDataFromUART();

void setup() {
    Serial.begin(115200);
    sensor.begin(9600);
    wifiManager.setup();
    timeManager.setup();
    awsManager.setup();
    // thingSpeakManager.setup();
    // firebaseModule.setup();
    // sensorManager.setup();
}

void loop() {
    if (readDataFromUART()) {
        JsonDocument doc;
        doc["time"] = millis();
        doc["humidity"] = humidity;
        doc["temperature"] = temperature;
        doc["soil moisture"] = soilMoisture;

        awsManager.publishMessage(doc.as<JsonObject>());
        // thingSpeakManager.sendData(temperature, humidity, soilMoisture);
        // firebaseModule.sendData(1, 1.0);
    }

    awsManager.loop();

    delay(5000); 
}

bool readDataFromUART() {
    if (sensor.available()) {
        JsonDocument doc;
        String data = sensor.readStringUntil('\n');
        DeserializationError error = deserializeJson(doc, data);
        if (!error) {
            humidity = doc["humidity"];
            temperature = doc["temperature"];
            soilMoisture = doc["soil moisture"];
        } else {
            Serial.println(error.c_str());
        }

        return true;
    }
    return false;
}