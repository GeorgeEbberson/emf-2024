#ifndef WIFICOM
#define WIFICOM

#include <FastLED.h>
#include <HTTPClient.h>

void initWiFi();
void loopWiFi(CRGB *color, HTTPClient *http);

#endif // WIFICOM
