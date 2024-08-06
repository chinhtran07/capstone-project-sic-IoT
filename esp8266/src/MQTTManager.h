#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

class MQTTManager {
public:
    MQTTManager(SoftwareSerial& serial);
    void setup();
    void loop();
    void publishMessage(const JsonObject& data);
private:
    WiFiClientSecure net;
    PubSubClient client;
    SoftwareSerial& serial;
    void connectAWS();
    void messageReceived(char* topic, byte* payload, unsigned int length);
};

#endif // MQTTMANAGER_H
