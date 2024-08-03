#include <Arduino.h>
#include "SensorModule.h"
#include "RelayModule.h"
#include "LCDModule.h"
#include "UARTModule.h"
#include <ArduinoJson.h>

#define DHT_PIN 8
#define DHT_TYPE DHT11
#define SOIL_MOISTURE_PIN A0
#define RELAY_PIN 13
#define BUTTON_PIN 3
#define RX_PIN 10
#define TX_PIN 11
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

SensorModule sensor(DHT_PIN, DHT_TYPE, SOIL_MOISTURE_PIN);
RelayModule relay(RELAY_PIN);
LCDModule lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
UARTModule uart(RX_PIN, TX_PIN);

volatile bool buttonPressed = false;

void handleButtonPress();

void setup()
{
  Serial.begin(9600);
  uart.begin(9600);
  sensor.begin();
  relay.begin();
  lcd.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonPress, FALLING);
}

void loop()
{
  if (uart.receive().length() > 0)
  {
    String received = uart.receive();
    Serial.print("Received: ");
    Serial.println(received);
  }

  sensor.readSensors();
  float temperature = sensor.getTemperature();
  float humidity = sensor.getHumidity();
  int soilMoisture = sensor.getSoilMoisture();

  if (isnan(humidity) || isnan(temperature))
  {
    lcd.displayMessage("DHTT11 Error");
    Serial.println("DHT11 Error: Invalid data");
  }
  else
  {
    lcd.displayData(temperature, humidity, soilMoisture);
    String json;
    JsonDocument doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["soil moisture"] = soilMoisture;
    serializeJson(doc, json);
    uart.send(json);
  }

  relay.setState(relay.getState());

  delay(1000);
}

void handleButtonPress()
{
  buttonPressed = true;
  delay(50);
  if (buttonPressed)
  {
    relay.toggle();
    buttonPressed = false;
    Serial.print("Relay State: ");
    Serial.println(relay.getState() ? "ON" : "OFF");
  }
}
