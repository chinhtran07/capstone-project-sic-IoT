#include <Arduino.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266Firebase.h>
#include <Ticker.h>
#include <EEPROM.h>

#define REFERENCE_URL "https://esp8266-b978f-default-rtdb.asia-southeast1.firebasedatabase.app/"

const int TIME_ZONE = 7;

WiFiUDP wifiUdp;
NTPClient ntpClient(wifiUdp, TIME_ZONE * 3600);
SoftwareSerial serial(D1, D2);
Firebase firebase(REFERENCE_URL);

float humidity, temperature;
int soilMoisture;

const int hourAddresses[3] = {0, 4, 8};
const int minuteAddresses[3] = {12, 16, 20};
const int secondAddresses[3] = {24, 28, 32};

bool previousStatus = false;
int control;
int relayStatus;

bool readDataFromUART();

void checkControl();
void pushDataToFirebase();
void updateWateringTime();
// void checkWateringTime();

Ticker timerContorl(checkControl, 1000);
Ticker timerPushData(pushDataToFirebase, 60000);
Ticker timerUpdateWateringTime(updateWateringTime, 30 * 60000);
// Ticker timerCheckWatertingTime(checkWateringTime, 60000);

void setup()
{
    Serial.begin(115200);
    serial.begin(9600);
    WiFiManager wm;

    Serial.print("Connecting");
    while (!wm.autoConnect("ESP8266"))
    {
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected");
    firebase.json(true);

    ntpClient.begin();
    timerContorl.start();
    timerPushData.start();
    updateWateringTime();
}

void loop()
{
    timerContorl.update();
    ntpClient.update();
    timerPushData.update();
    if (serial.available())
    {
        JsonDocument doc;
        String data = serial.readStringUntil('\n');
        DeserializationError error = deserializeJson(doc, data);
        if (!error && doc["relay_status"] != nullptr)
        {
            relayStatus = doc["relay_status"].as<String>() == "ON";
            firebase.setInt("control", relayStatus);
            previousStatus = relayStatus;
            Serial.println(relayStatus);
        }
    }
}

bool readDataFromUART()
{
    if (serial.available())
    {
        JsonDocument doc;
        String data = serial.readStringUntil('\n');
        DeserializationError error = deserializeJson(doc, data);
        if (!error)
        {
            humidity = doc["humidity"];
            temperature = doc["temperature"];
            soilMoisture = doc["soil moisture"];
        }
        else
        {
            Serial.println(error.c_str());
        }

        return true;
    }
    return false;
}

void checkControl()
{
    ntpClient.update();
    control = firebase.getInt("control");

    if (control != previousStatus)
    {
        Serial.println(control == 1 ? "RELAY OFF" : "RELAY ON");
        serial.println(control);
        previousStatus = control;
    }
}

void pushDataToFirebase()
{
    if (readDataFromUART())
    {
        long time = ntpClient.getEpochTime();
        String path = "sensor/" + String(time);
        firebase.pushFloat(path + "/temperature", temperature);
        firebase.pushFloat(path + "/humidity", humidity);
        firebase.pushInt(path + "/soilMoisture", soilMoisture);
    }
}

void updateWateringTime()
{
    String wateringTime = firebase.getString("wateringTimer");

    JsonDocument doc;

    deserializeJson(doc, wateringTime);
    String timer1 = doc["timer1"];
    int indexes[3] = {1, 2, 3};
    for (int i = 0; i < 3; ++i)
    {
        String timePath = "timer" + String(indexes[i]);
        String timeString = doc[timePath];
        int hour = timeString.substring(0, 2).toInt();
        int minute = timeString.substring(3, 5).toInt();
        int second = timeString.substring(6, 8).toInt();

        EEPROM.begin(512);
        EEPROM.write(hourAddresses[i], hour);
        EEPROM.write(minuteAddresses[i], minute);
        EEPROM.write(secondAddresses[i], second);
        EEPROM.commit();
        Serial.println("Done" + String(indexes[i]));
    }
}
