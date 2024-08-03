#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

class MQTTManager {
public:
    MQTTManager();
    void setup();
    void loop();
    void publishMessage(const JsonObject& data);
    void messageReceived(char* topic, uint8_t* payload, unsigned int length);
private:
    WiFiClientSecure net;
    PubSubClient client;
    void connectAWS();
};

#endif // MQTTMANAGER_H
