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

int test_generate_random_data() {
    int foo1 = 1;

    for (int i = 0; i < 100; i++) {
        int32_t data = generate_random_data();

        if (data > 0xFFF) {
            foo1 = 0;
            return EXIT_FAILURE;
        }
    }

    return foo1;
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

int test_swap_be() {
    int foo2 = 1;
    uint32_t input = generate_random_data();
    uint32_t result = swap_be(input);

    //printf("Input:  0x%08X\n", input);
    //printf("Result: 0x%08X\n", result);


    uint8_t first_byte_input = (input >> 24) & 0xFF;
    uint8_t fourth_byte_result = result & 0xFF;


    if (first_byte_input != fourth_byte_result) {
        foo2 = 0;
    }

    return foo2;
}

