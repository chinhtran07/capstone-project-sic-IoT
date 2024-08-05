#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include <DHT.h>

class SensorModule {
  private:
    DHT* dht;
    int soilMoisturePin;
    float temperature;
    float humidity;
    int soilMoisture;

  public:
    SensorModule(int dhtPin, int dhtType, int soilMoisturePin);
    void begin();
    void readSensors();
    float getTemperature();
    float getHumidity();
    int getSoilMoisture();
};

#endif
