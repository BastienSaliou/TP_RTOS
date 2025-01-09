#include "byte.h"
#include <stdlib.h>
#include <stdint.h>

uint32_t generate_random_data() {
    return (rand() % (1 << 24)) - (1 << 23);
}

uint32_t to_big_endian(uint32_t data) {
    return ((data & 0xFF0000) >> 16) | ((data & 0x00FF00)) | ((data & 0x0000FF) << 16);
}

