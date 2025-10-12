#ifndef MEMCPY_LONG_H
#define MEMCPY_LONG_H
#include <stdint.h>
 
static inline void memcpy_long(uint32_t* dest, const uint32_t* src, size_t length) {
    while (length--) {
        *dest = *src;
        dest++;
        src++;
    }
}

#endif
