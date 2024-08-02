#include <Arduino.h>
#include "WiFiManagerWrapper.h"
#include "MQTTManager.h"
// #include "ThingSpeakManager.h"
// #include "FirebaseManager.h"
#include "TimeManager.h"
#include "SensorManager.h"

WiFiManagerWrapper wifiManager;
MQTTManager awsManager;
// ThingSpeakManager thingSpeakManager;
// FirebaseManager firebaseManager;
TimeManager timeManager;
SensorManager sensorManager;

void setup() {
    Serial.begin(115200);
    wifiManager.setup();
    timeManager.setup();
    awsManager.setup();
    // thingSpeakManager.setup();
    // firebaseManager.setup();
    sensorManager.setup();
}

void loop() {
    float humidity, temperature;
    int soilMoisture;

    if (sensorManager.readSensors(humidity, temperature, soilMoisture)) {
        Serial.print(F("Humidity: "));
        Serial.println(humidity);
        Serial.print(F("Temperature: "));
        Serial.println(temperature);
        Serial.print(F("Soil Moisture: "));
        Serial.println(soilMoisture);

        // Create JSON object to send to AWS
        StaticJsonDocument<200> doc;
        doc["time"] = millis();
        doc["humidity"] = humidity;
        doc["temperature"] = temperature;
        doc["soil moisture"] = soilMoisture;

        // Publish data to AWS
        awsManager.publishMessage(doc.as<JsonObject>());

        // Send data to ThingSpeak
        // thingSpeakManager.sendData(soilMoisture);

        // Send data to Firebase
        // firebaseManager.sendData("/soilMoisture", soilMoisture);
    } else {
        Serial.println("Failed to read data from sensor Arduino");
    }

    // Loop AWS Manager
    awsManager.loop();

    delay(20000); // Delay to prevent overloading services
}
