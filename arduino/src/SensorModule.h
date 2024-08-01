#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include <DHT.h>

#define DHTPIN 8
#define DHTTYPE DHT11
#define MOISTURE_PIN A0

class SensorModule {
  private:
    DHT dht;
    float temperature;
    float humidity;
    int soilMoisture;

  public:
    SensorModule();
    void begin();
    void readSensors();
    void printData();
    float getTemperature();
    float getHumidity();
    int getSoilMoisture();
};

#endif
