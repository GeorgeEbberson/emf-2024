#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdint.h>

#define VEC3 (3u) // Num elements in 3-vector

// To make queue actions non-blocking (either work now or fail, don't wait)
#define NON_BLOCKING (0u)

// Numbers hardcoded to match the cloud-side
typedef enum {
    Mode_None = 0u,
    Mode_Normal = 1,
    Mode_Rave = 2,
    Mode_Pulse = 3,
    Mode_Spectrum = 4,
    Mode_Rainbow = 5,
    Mode_MAX
} Mode_t;

typedef struct ThreadMsg_s {
    Mode_t mode;
    uint8_t rgb[VEC3];
    uint8_t brightness;
} ThreadMsg_t;

#endif // COMMON_HPP
