#include <stdint.h>
#include <stdlib.h>
#include "Byte.h"
#include "Test_lib.h"
#include "package.h"

int test_generate_random_data() {
    int foo1 = 1;

    for (int i = 0; i < 100; i++) {
        int32_t data = generate_random_data();

        if (data > 0xFFFFFF) {
            foo1 = 0;
            return EXIT_FAILURE;
        }
    }

    return foo1;
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

int main(void) {

    tst_t tests[] = {
            DECL_TEST(test_generate_random_data),
            DECL_TEST(test_swap_be),
            DECL_TEST(test_generate_package),
            LAST_TEST //Sentinelle
    };

    printf("Starting Tests...\n");
    tst_all(tests);
    printf("All Tests Completed.\n");

    return EXIT_SUCCESS;
}