#include "MQTTManager.h"
#include <time.h>
#include "secrets.h"


#define AWS_IOT_PUBLISH_TOPIC   "esp8266/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub"

unsigned long lastMillis = 0;

BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

MQTTManager::MQTTManager() : client(net) {}

void MQTTManager::setup() {
    net.setTrustAnchors(&cert);
    net.setClientRSACert(&client_crt, &key);
    client.setServer(MQTT_HOST, 8883);
    client.setCallback([this](char *topic, uint8_t *payload, unsigned int length)
                       { this->messageReceived(topic, payload, length); });
    connectAWS();
}

void MQTTManager::loop() {
    if (!client.connected()) {
        connectAWS();
    } else {
        client.loop();
        if (millis() - lastMillis > 5000) {
            lastMillis = millis();
        }
    }
}

void MQTTManager::publishMessage(const JsonObject& data) {
    char jsonBuffer[512];
    serializeJson(data, jsonBuffer);
    client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void MQTTManager::connectAWS() {
    Serial.println("Connecting to AWS IoT");
    while (!client.connected()) {
        if (client.connect(THINGNAME)) {
            Serial.println("Connected to AWS IoT");
            client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
        } else {
            Serial.print("Failed to connect, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void MQTTManager::messageReceived(char* topic, uint8_t* payload, unsigned int length) {
    Serial.print("Received [");
    Serial.print(topic);
    Serial.print("]: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}
