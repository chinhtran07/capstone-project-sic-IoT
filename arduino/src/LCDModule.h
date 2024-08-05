#ifndef LCD_MODULE_H
#define LCD_MODULE_H

#include <LiquidCrystal_I2C.h>

class LCDModule {
  private:
    LiquidCrystal_I2C lcd;

  public:
    LCDModule(uint8_t address, uint8_t columns, uint8_t rows);
    void begin();
    void displayData(float temperature, float humidity, int soilMoisture);
    void displayMessage(const char *message);
};

#endif
