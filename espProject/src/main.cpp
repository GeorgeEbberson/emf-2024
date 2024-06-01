
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
    static int mode;
    static CRGB color;
    static int brightness;
    HTTPClient http;
    loopWiFi(&mode, &color, &brightness, &http);
    ledset(&mode, &color, &brightness);
    delay(1000);
}