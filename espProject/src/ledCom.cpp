#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN 26
#define NUM_LEDS 600

CRGB leds[NUM_LEDS];

void setLed(CRGB *led, int r, int g, int b)
{
    led->r = r;
    led->g = g;
    led->b = b;
}

void ledsetup()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setCorrection(LEDColorCorrection::TypicalPixelString);
    setLed(&leds[0], 255, 0, 0);
    setLed(&leds[1], 0, 255, 0);
    setLed(&leds[2], 0, 0, 255);
    FastLED.show();

}

void ledset(CRGB *color)
{
    // Set all LEDs to selected color
    for (int i = 1; i < NUM_LEDS; i++)
    {
        leds[i] = *color;
    }

    // Tick First LED as a keepalive
    if (leds[0].r == 0 && leds[0].g == 0 && leds[0].b == 0)
    {
        leds[0] = *color;
    }
    else
    {
        setLed(&leds[0], 0, 0, 0);
    }
    // Serial.printf("Wang: R: %u G: %u B: %u into LEDs\n", color->r, color->g, color->b);
    FastLED.show();
}
