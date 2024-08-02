#include "SensorManager.h"
#include <SoftwareSerial.h>

// Configure SoftwareSerial on pins D1 (RX) and D2 (TX)
SoftwareSerial sensorSerial(D1, D2);

void SensorManager::setup() {
    sensorSerial.begin(9600);
}

bool SensorManager::readSensors(float& humidity, float& temperature, int& soilMoisture) {
    if (sensorSerial.available()) {
        String data = readSerialData();
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

String SensorManager::readSerialData() {
    String data = "";
    while (sensorSerial.available()) {
        char ch = sensorSerial.read();
        if (ch == '\n') break;
        data += ch;
    }
    return data;
}
