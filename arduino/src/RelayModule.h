#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

class RelayModule {
  private:
    int relayPin;
    bool relayState;

  public:
    RelayModule(int relayPin);
    void begin();
    void toggle();
    void setState(bool state);
    bool getState();
};

#endif
