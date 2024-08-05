#ifndef THINGSPEAKMANAGER_H
#define THINGSPEAKMANAGER_H

#include <ESP8266WiFi.h>

class ThingSpeakManager {
public:
    ThingSpeakManager();
    void setup();
    void sendData(float temperature, float humidity, int soilMoisture);
private:
    const char* SERVER = "api.thingspeak.com";
    const char* WRITE_API_KEY = "IPH2AVE2AJQLR6CP";
    const long CHANEL_ID = 2615853;
};

#endif // THINGSPEAKMANAGER_H
