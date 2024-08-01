#include <Arduino.h>
#include <SensorModule.h>

SensorModule sensorModule;

void setup() {
  Serial.begin(9600);
  sensorModule.begin();
}

void loop() {
  sensorModule.readSensors();
  sensorModule.printData();
}

