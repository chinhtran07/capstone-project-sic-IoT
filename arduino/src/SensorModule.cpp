#include "SensorModule.h"

SensorModule::SensorModule(int dhtPin, int dhtType, int soilMoisturePin) {
  dht = new DHT(dhtPin, dhtType);
  this->soilMoisturePin = soilMoisturePin;
}

void SensorModule::begin() {
  dht->begin();
}

void SensorModule::readSensors() {
  temperature = dht->readTemperature();
  humidity = dht->readHumidity();
  soilMoisture = analogRead(soilMoisturePin);
}

float SensorModule::getTemperature() {
  return temperature;
}

float SensorModule::getHumidity() {
  return humidity;
}

int SensorModule::getSoilMoisture() {
  return soilMoisture;
}
