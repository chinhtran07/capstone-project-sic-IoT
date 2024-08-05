#include "WiFiManagerWrapper.h"

void WiFiManagerWrapper::setup() {
    WiFiManager wifiManager;
    wifiManager.autoConnect("AutoConnectAP"); // Tên AP khi không có WiFi
    Serial.println("Connected to WiFi");
}
