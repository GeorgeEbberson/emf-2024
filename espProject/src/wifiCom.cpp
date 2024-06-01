
#include <stdint.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <FastLED.h>

#include "common.hpp"
#include "ledCOM.hpp"

#define SEND_WAIT_MS (50u)
#define SEND_WAIT_TICKS ((TickType_t)(SEND_WAIT_MS / portTICK_PERIOD_MS))

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

void loopWiFi(ThreadMsg_t *msg, HTTPClient *http)
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

        msg = (ThreadMsg_t *)&(parcelChar[0]);

        // msg->mode = parcelChar[0];
        // msg->rgb[0] = parcelChar[1];
        // msg->rgb[1] = parcelChar[2];
        // msg->rgb[2] = parcelChar[3];
        // msg->brightness = parcelChar[4];

        Serial.printf(" - Packet: Mode: %u R: %u G: %u B: %u Brightness: %u into LEDs\n",
        msg->mode, msg->rgb[0], msg->rgb[1], msg->rgb[2], msg->brightness);
    }
    else
    {
        Serial.printf("Error code: %u\n", httpResponseCode);
    }
}

void NetworkMain(void *param)
{
    Serial.println("Started network thread");
    QueueHandle_t queue = (QueueHandle_t)param;

    HTTPClient http;

    while (true)
    {
        delay(1000);
        ThreadMsg_t msg;
        loopWiFi(&msg, &http);

        UBaseType_t spaces = uxQueueSpacesAvailable(queue);
        if (spaces > 0)
        {
            xQueueSendToBack(queue, &msg, SEND_WAIT_TICKS);
        }
    }
}
