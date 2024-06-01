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

void ledset(int *mode, CRGB *color, int *brightness)
{
    static int frameCounter;  // reset this when mode changes?
    switch (*mode)
    {
        case Mode_Normal:  // Normal
        {
            FastLED.setBrightness(*brightness);
            // Set all LEDs to selected color
            for (int i = 1; i < NUM_LEDS; i++)
            {
                leds[i] = *color;
            }
            break;
        }
        case Mode_Rave:  // Forced Rave
        {
            FastLED.setBrightness(*brightness);
            color->r = random(0,3)*32;
            color->g = random(0,3)*32;
            color->b = random(0,3)*32;
            
            // Set all LEDs to selected color
            for (int i = 1; i < NUM_LEDS; i++)
            {
                leds[i] = *color;
            }
            break;
        }
        case Mode_Pulse:  // Color Pulse
        {
            FastLED.setBrightness(*brightness * sin(2 * PI * (frameCounter % 50)/50));
            // Set all LEDs to selected color
            for (int i = 1; i < NUM_LEDS; i++)
            {
                leds[i] = *color;
            }
            break;
        }
        case Mode_Spectrum:  // Spectrum Wave
        {
            break;
        }
        case Mode_Rainbow:  // Static Rainbow
        {
            break;
        }
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
    Serial.printf("Wang: Mode: %u R: %u G: %u B: %u Brightness: %u into LEDs\n", *mode, color->r, color->g, color->b, *brightness);
    
    // Pleanery
    frameCounter++;
    FastLED.show();

}
