#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <Arduino.h>

class SensorManager {
public:
    void setup();
    bool readSensors(float& humidity, float& temperature, int& soilMoisture);
private:
    String readSerialData();
};

#endif // SENSORMANAGER_H
