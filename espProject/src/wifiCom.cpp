
#include <stdint.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <FastLED.h>

#include "ledCOM.hpp"

const char* ssid = "emf2024-open";
const char* password = "";
const String server = "http://calls.alexnielsen.me/led";  //95.216.180.212/led/constants

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
}

void loopWiFi(CRGB *color)
{
    int parcel = -1;

    if (WiFi.status() != WL_CONNECTED)
    {
        initWiFi();
    }

    HTTPClient http;
    http.begin(server.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.print(httpResponseCode);
        // Has to be like this else it is bad
        String parcelString = http.getString();
        const char *parcelChar = parcelString.c_str();

        color->r = parcelChar[0];
        color->g = parcelChar[1];
        color->b = parcelChar[2];
        Serial.printf(" - Packet: R: %u G: %u B: %u\n", color->r, color->g, color->b);
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}

