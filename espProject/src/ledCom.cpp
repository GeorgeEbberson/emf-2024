#include <stdint.h>
#include <Arduino.h>
#include <FastLED.h>

#include "common.hpp"

#define DATA_PIN 26
#define NUM_LEDS 600
#define ANIM_SPEED 50

#define LED_PERIOD_MS (100u)
#define LED_PERIOD_TICKS ((TickType_t)(LED_PERIOD_MS / portTICK_PERIOD_MS))

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

void ledset(int mode, CRGB *color, int brightness)
{
    static int frameCounter;  // reset this when mode changes?
    static bool blinkFlag;
    switch (mode)
    {
        case Mode_Normal:  // Normal
        {
            FastLED.setBrightness(brightness);
            // Set all LEDs to selected color
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = *color;
            }
            break;
        }
        case Mode_Rave:  // Forced Rave
        {
            FastLED.setBrightness(brightness);
            color->r = random(0,3)*32;
            color->g = random(0,3)*32;
            color->b = random(0,3)*32;

            // Set all LEDs to selected color
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = *color;
            }
            break;
        }
        case Mode_Pulse:  // Color Pulse
        {
            brightness = (int)(
                (float)brightness / 2.0f)
                * ((float)sin((2 * PI * (float)(frameCounter % ANIM_SPEED))/ float(ANIM_SPEED))
                + 1.0f
              );
                        
            FastLED.setBrightness(brightness);
            // Set all LEDs to selected color (except first)
            for (int i = 0; i < NUM_LEDS; i++)
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
    if (blinkFlag)
    {
        setLed(&leds[0], 0, 0, 0);
    }
    else
    {
        leds[0] = *color;
    }
    blinkFlag = !blinkFlag;

    // Serial.printf("Wang: Mode: %u R: %u G: %u B: %u Brightness: %u into LEDs\n",
    // mode, color->r, color->g, color->b, brightness);

    Serial.printf("Wang: Mode: %u R: %u G: %u B: %u Brightness: %u into LEDs\n",
    mode, color->r, color->g, color->b, brightness);

    // Pleanery
    frameCounter++;
    FastLED.show();

}

void ledstep(const ThreadMsg_t *msg)
{
    CRGB color;
    color.r = msg->rgb[0u];
    color.g = msg->rgb[1u];
    color.b = msg->rgb[2u];
    ledset(msg->mode, &color, msg->brightness);
}

void LedMain(void *param)
{
    Serial.println("Started LED thread");
    QueueHandle_t queue = (QueueHandle_t)param;
    ledsetup();

    ThreadMsg_t lastMsg;

    TickType_t lastWakeTime = xTaskGetTickCount();
    while (true)
    {
        vTaskDelayUntil(&lastWakeTime, LED_PERIOD_TICKS);
        ThreadMsg_t newMsg;
        BaseType_t result = xQueueReceive(queue, &newMsg, NON_BLOCKING);
        if (result == pdTRUE)
        {
            lastMsg = newMsg;
        }
        ledstep(&lastMsg);
    }
}
