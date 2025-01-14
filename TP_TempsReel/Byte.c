#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>



unsigned char rand8(){
    static int initialized = 0;
    if (!initialized){

        initialized =! initialized;
    }
    return rand()&0xFF;
}

int8_t generate_random_data() {
    return (rand8() % 0xFF);
}


int swap_be_8(uint8_t *data) {

    uint8_t temp = data[0];
    data[0] = data[2];
    data[2] = temp;
    return *data;
}
