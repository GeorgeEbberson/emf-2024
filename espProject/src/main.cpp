
#include <Arduino.h>
#include <FastLED.h>
#include <HTTPClient.h>

#include "wifiCom.hpp"
#include "ledCOM.hpp"

void setup() {
    Serial.begin(9600);
    ledsetup();
    Serial.print("Setup done");
}

void loop() {
    static CRGB color;
    HTTPClient http;
    loopWiFi(&color, &http);
    ledset(&color);
    delay(1000);
}