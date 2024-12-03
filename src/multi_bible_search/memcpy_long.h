#ifndef MEMCPY_LONG_H
#define MEMCPY_LONG_H

static inline void memcpy_long(long* dest, const long* src, size_t length) {
    while (length--) {
        *dest = *src;
        dest++;
        src++;
    }
}

#endif