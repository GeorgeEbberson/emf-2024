#ifndef WIFICOM
#define WIFICOM

#include <FastLED.h>
#include <HTTPClient.h>

void initWiFi();
void loopWiFi(int *mode, CRGB *color, int *brightness, HTTPClient *http);

#endif // WIFICOM
