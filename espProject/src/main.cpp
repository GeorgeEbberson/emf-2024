#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN 26
#define NUM_LEDS 600
#define COLOR_NUM 5

CRGB leds[NUM_LEDS];
CRGB colors[COLOR_NUM] = {CRGB::White, CRGB::Black, CRGB::Red, CRGB::Green, CRGB::Blue};

void setup()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop()
{
    static int idx = 0;

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = colors[idx];
    }
    idx = (idx + 1) % COLOR_NUM;
    FastLED.show();
    delay(1000);
}
