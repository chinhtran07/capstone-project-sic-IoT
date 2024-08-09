  #include "RelayModule.h"
  #include <Arduino.h>

  RelayModule::RelayModule(int relayPin) : relayPin(relayPin), relayState(false) {}

  void RelayModule::begin() {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
  }

  void RelayModule::toggle() {
    relayState = !relayState;
    digitalWrite(relayPin, relayState ? HIGH : LOW);
  }

  void RelayModule::setState(bool state) {
    relayState = state;
    digitalWrite(relayPin, relayState ? HIGH : LOW);
  }

  bool RelayModule::getState() {
    return relayState;
  }
