#include "LCDModule.h"
#include <Wire.h>

LCDModule::LCDModule(uint8_t address, uint8_t columns, uint8_t rows) : lcd(address, columns, rows) {}

void LCDModule::begin() {
  lcd.init();
  lcd.backlight();
}

void LCDModule::displayData(float temperature, float humidity, int soilMoisture) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C ");
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Soil: ");
  lcd.print(soilMoisture);
}


void LCDModule::displayMessage(const char* message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
}
