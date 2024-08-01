#include <SensorModule.h>

SensorModule::SensorModule() : dht(DHTPIN, DHTTYPE) {}

void SensorModule::begin()
{
    dht.begin();
}

void SensorModule::readSensors()
{
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    soilMoisture = analogRead(MOISTURE_PIN);
}

void SensorModule::printData() {
    if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(", Hum: ");
  Serial.print(humidity);
  Serial.print(", Soil: ");
  Serial.println(soilMoisture);
}

float SensorModule::getHumidity() {
    return humidity;
}

float SensorModule::getTemperature() {
    return temperature;
}

int SensorModule::getSoilMoisture() {
    return soilMoisture;
}