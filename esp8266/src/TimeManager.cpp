#include "TimeManager.h"
#include <Arduino.h>
#include <time.h>

time_t now;
time_t nowish = 1510592825;

const int TIME_ZONE = 7;

void TimeManager::setup() {
    Serial.print("Setting time using SNTP");
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    now = time(nullptr);
    while (now < nowish) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("done!");
    now += TIME_ZONE * 3600;
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}
