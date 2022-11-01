//
// Modefy by Jiadong Xiao on 2021/6/20.
//

#include "io.h"
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

uint8_t* Load8(uint8_t *p, uint8_t x) {
    *p = x;
    return p+1;
}

uint8_t* Load16(uint8_t *p, uint16_t x) {
    p = Load8(p, (uint8_t)(x >> 8));
    p = Load8(p, (uint8_t)x);
    return p;
}

uint8_t* Load32(uint8_t *p, uint32_t x) {
    p = Load16(p, (uint16_t)(x >> 16));
    p = Load16(p, (uint16_t)x);
    return p;
}

