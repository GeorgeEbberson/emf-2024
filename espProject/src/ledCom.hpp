#ifndef LEDCOM
#define LEDCOM

#include <FastLED.h>

void ledsetup();
void ledset(int *mode, CRGB *color, int *brightness);

#endif // LEDCOM