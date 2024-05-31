
#include <Arduino.h>
#include <FastLED.h>

#include "wifiCom.hpp"
#include "ledCOM.hpp"

void setup() {
    Serial.begin(9600);
    ledsetup();
    Serial.print("Setup done");
}

void loop() {
    CRGB color;
    loopWiFi(&color);
    ledset(&color);
    delay(1000);
}