#include <Arduino.h>
#include "SensorModule.h"
#include "RelayModule.h"
#include "LCDModule.h"
#include "UARTModule.h"
#include <ArduinoJson.h>
#include <string.h>
#include <Ticker.h>

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
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;        // 50ms debounce delay
bool lastButtonState = HIGH;                   // Lưu trạng thái trước đó của nút nhấn
unsigned long timeBeginWatering = 0;           // Thời gian bắt đầu tưới
const unsigned long timeWatering = 10 * 60000; // Thời gian tưới (10 phút)
bool isWatering = false;                       // Trạng thái tưới nước

void handleButtonPress();
void updateRelayStatus();

void setup()
{
  Serial.begin(9600);
  uart.begin(9600);
  sensor.begin();
  relay.begin();
  lcd.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonPress, CHANGE);
}

void loop()
{

  String signal;
  if (uart.receive(signal))
  {
    int control = signal.toInt();
    relay.setState(control);
    Serial.println(relay.getState() ? "ON" : "OFF");
    isWatering = control;
  }

  // Nếu đang tưới nước, kiểm tra nếu đã hết thời gian tưới
  if (isWatering)
  {
    unsigned long currentTime = millis();
    if (currentTime - timeBeginWatering >= timeWatering)
    {
      // Dừng tưới nước
      relay.setState(false);
      isWatering = false;
      Serial.println("Watering completed.");
      updateRelayStatus();
    }
    // Không kiểm tra độ ẩm khi đang tưới nước
    return;
  }

  // Đọc giá trị cảm biến
  sensor.readSensors();
  float temperature = sensor.getTemperature();
  float humidity = sensor.getHumidity();
  int soilMoisture = sensor.getSoilMoisture();

  // Kiểm tra dữ liệu cảm biến DHT11
  if (isnan(temperature) || isnan(humidity))
  {
    lcd.displayMessage("DHT11 Error");
    Serial.println("DHT11 Error: Invalid data");
  }
  else
  {
    lcd.displayData(temperature, humidity, soilMoisture);

    // Tạo và gửi dữ liệu JSON qua UART
    JsonDocument doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["soil_moisture"] = soilMoisture;
    String json;
    serializeJson(doc, json);
    uart.send(json);
  }

  // Điều khiển relay dựa trên độ ẩm đất
  if (soilMoisture > 600)
  {
    relay.setState(true);
    timeBeginWatering = millis(); // Ghi lại thời gian bắt đầu tưới
    isWatering = true;            // Đánh dấu trạng thái tưới nước
    Serial.println("Watering started.");
    updateRelayStatus();
  }

  // Đợi trước khi lặp lại
  delay(1000);
}

void handleButtonPress()
{
  unsigned long currentTime = millis();
  bool buttonState = digitalRead(BUTTON_PIN);

  // Chỉ xử lý khi có sự thay đổi trạng thái nút nhấn
  if (buttonState != lastButtonState && (currentTime - lastDebounceTime) > debounceDelay)
  {
    lastDebounceTime = currentTime;

    if (buttonState == LOW)
    {
      relay.toggle();
      Serial.print("Relay State: ");
      Serial.println(relay.getState() ? "ON" : "OFF");
      isWatering = relay.getState();
      updateRelayStatus();
    }
  }

  lastButtonState = buttonState; // Cập nhật trạng thái nút nhấn
}

void updateRelayStatus()
{
  JsonDocument doc;
  doc["relay_status"] = relay.getState() ? "ON" : "OFF";
  String data;
  serializeJson(doc, data);
  uart.send(data);
  Serial.println(data);
}