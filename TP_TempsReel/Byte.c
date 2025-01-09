#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

unsigned char rand12(){
    static int initialized = 0;
    if (!initialized){
        srand(time(NULL));
        initialized =! initialized;
    }
    return rand()&0xFFF;
}

int32_t generate_random_data() {
    return (rand12() % 0xFFF);
}



uint32_t swap_be(uint32_t data) {
    uint8_t *bytes = (uint8_t *)&data;

    uint8_t temp = bytes[0];
    bytes[0] = bytes[3];
    bytes[3] = temp;

    temp = bytes[1];
    bytes[1] = bytes[2];
    bytes[2] = temp;

    return data;
}
