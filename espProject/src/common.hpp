#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdint.h>

#define VEC3 (3u) // Num elements in 3-vector

typedef enum {
    Mode_None,
    Mode_Normal,
    Mode_Rave,
    Mode_MAX
} Mode_t;

typedef struct ThreadMsg_s {
    Mode_t mode;
    uint8_t rgb[VEC3];
    uint8_t brightness;
} ThreadMsg_t;

#endif // COMMON_HPP
