#include "ThingSpeakManager.h"
#include <ThingSpeak.h>

WiFiClient client;

ThingSpeakManager::ThingSpeakManager() {}

void ThingSpeakManager::setup() {
    ThingSpeak.begin(client);
}

void ThingSpeakManager::sendData(float temperature, float humidity, int soilMoisture) {
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    ThingSpeak.setField(3, soilMoisture);
    int httpCode = ThingSpeak.writeFields(CHANEL_ID, WRITE_API_KEY);
    if (httpCode == 200) {
        Serial.println("Data sent to ThingSpeak successfully");
    } else {
        Serial.println("Problem sending data to ThingSpeak: HTTP code " + String(httpCode));
    }
}
