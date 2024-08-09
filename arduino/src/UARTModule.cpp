#include "UARTModule.h"

UARTModule::UARTModule(int rxPin, int txPin) {
  mySerial = new SoftwareSerial(rxPin, txPin);
}

void UARTModule::begin(long baudRate) {
  mySerial->begin(baudRate);
}

void UARTModule::send(String message) {
  mySerial->println(message);
}

bool UARTModule::receive(String &signal) {
  if (mySerial->available()) {
    signal = mySerial->readStringUntil('\n');
    return true;
  }
  return false;
}
