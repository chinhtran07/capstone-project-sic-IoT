#ifndef UART_MODULE_H
#define UART_MODULE_H

#include <SoftwareSerial.h>

class UARTModule {
  private:
    SoftwareSerial* mySerial;

  public:
    UARTModule(int rxPin, int txPin);
    void begin(long baudRate);
    void send(String message);
    String receive();
};

#endif
