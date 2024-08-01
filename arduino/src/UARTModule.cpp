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

String UARTModule::receive() {
  if (mySerial->available()) {
    return mySerial->readString();
  }
  return "";
}
