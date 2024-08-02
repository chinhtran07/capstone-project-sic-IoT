#include <Arduino.h>
#include "WiFiManagerWrapper.h"
#include "MQTTManager.h"
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
        StaticJsonDocument<200> doc;
        doc["time"] = millis();
        doc["humidity"] = humidity;
        doc["temperature"] = temperature;
        doc["soil moisture"] = soilMoisture;

        awsManager.publishMessage(doc.as<JsonObject>());
    }

    // if (sensorManager.readSensors(humidity, temperature, soilMoisture)) {
    //     Serial.print(F("Humidity: "));
    //     Serial.println(humidity);
    //     Serial.print(F("Temperature: "));
    //     Serial.println(temperature);
    //     Serial.print(F("Soil Moisture: "));
    //     Serial.println(soilMoisture);

    //     // Create JSON object to send to AWS
    //     StaticJsonDocument<200> doc;
    //     doc["time"] = millis();
    //     doc["humidity"] = humidity;
    //     doc["temperature"] = temperature;
    //     doc["soil moisture"] = soilMoisture;

    //     // Publish data to AWS
    //     awsManager.publishMessage(doc.as<JsonObject>());

    //     // Send data to ThingSpeak
    //     // thingSpeakManager.sendData(soilMoisture);

    //     // Send data to Firebase
    //     // firebaseManager.sendData("/soilMoisture", soilMoisture);
    // } else {
    //     Serial.println("Failed to read data from sensor Arduino");
    // }

    // Loop AWS Manager
    awsManager.loop();

    delay(1000); // Delay to prevent overloading services
}

bool readDataFromUART(float &humidity, float &temperature, int &soilMoisture) {
    if (sensor.available()) {
        String data = sensor.readString();
        int hIndex = data.indexOf("Humidity:");
        int tIndex = data.indexOf("Temperature:");
        int sIndex = data.indexOf("SoilMoisture:");

        if (hIndex != -1 && tIndex != -1 && sIndex != -1) {
            humidity = data.substring(hIndex + 9, data.indexOf(';', hIndex)).toFloat();
            temperature = data.substring(tIndex + 12, data.indexOf(';', tIndex)).toFloat();
            soilMoisture = data.substring(sIndex + 12).toInt();
            return true;
        }
    }
    return false;
}