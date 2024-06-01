
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

void loopWiFi(int *mode, CRGB *color, int *brightness, HTTPClient *http)
{
    int parcel = -1;

    if (WiFi.status() != WL_CONNECTED)
    {
        initWiFi();
    }

    if (!http->connected())
    {
        http->end();
        http->begin(server.c_str());
    }
    int httpResponseCode = http->GET();

    if (httpResponseCode > 0) {
        Serial.printf("HTTP Response code: %u\n", httpResponseCode);
        // Has to be like this else it is bad
        String parcelString = http->getString();
        const char *parcelChar = parcelString.c_str();

        // Unpack message
        *mode = parcelChar[0];
        color->r = parcelChar[1];
        color->g = parcelChar[2];
        color->b = parcelChar[3];
        *brightness = parcelChar[4];
        Serial.printf(" - Packet: Mode: %u R: %u G: %u B: %u Brightness: %u into LEDs\n", *mode, color->r, color->g, color->b, *brightness);
    }
    else
    {
        Serial.printf("Error code: %u\n", httpResponseCode);
    }
}

