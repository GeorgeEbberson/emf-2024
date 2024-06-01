
#include <stdint.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <FastLED.h>

#include "common.hpp"
#include "ledCOM.hpp"

#define POLL_WAIT_MS (1000u)
#define POLL_PERIOD_TICKS ((TickType_t)(POLL_WAIT_MS / portTICK_PERIOD_MS))

const char* ssid = "emf2024-open";
const char* password = "";
const String server = "http://calls.alexnielsen.me/led";  //95.216.180.212/led/constants

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.printf("\nConnecting to WiFi...");
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
        DEBUG_LOG("HTTP Response code: %u\n", httpResponseCode);
        // Has to be like this else it is bad
        String parcelString = http->getString();
        const char *parcelChar = parcelString.c_str();

        // Unpack Data from parcel
        msg->mode = (Mode_t)parcelChar[0];
        msg->rgb[0] = (uint8_t)parcelChar[1];
        msg->rgb[1] = (uint8_t)parcelChar[2];
        msg->rgb[2] = (uint8_t)parcelChar[3];
        msg->brightness = (uint8_t)parcelChar[4];

        DEBUG_LOG("Rx: Mode: %u RGBA: %u,%u,%u,%u\n",
            msg->mode, msg->rgb[0], msg->rgb[1], msg->rgb[2], msg->brightness);
    }
    else
    {
        DEBUG_LOG("Error code: %u\n", httpResponseCode);
    }
}

void NetworkMain(void *param)
{
    Serial.println("Started network thread");
    QueueHandle_t queue = (QueueHandle_t)param;

    HTTPClient http;

    TickType_t lastWakeTime = xTaskGetTickCount();
    while (true)
    {
        vTaskDelayUntil(&lastWakeTime, POLL_PERIOD_TICKS);
        ThreadMsg_t msg;
        loopWiFi(&msg, &http);

        UBaseType_t spaces = uxQueueSpacesAvailable(queue);
        if (spaces > 0)
        {
            xQueueSendToBack(queue, &msg, NON_BLOCKING);
        }
    }
}
