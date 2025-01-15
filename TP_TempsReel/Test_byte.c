#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "Byte.h"
#include "Test_lib.h"

// Vérifie que les données générées restent dans la plage [0x00, 0xFF]
int test_generate_random_data() {
    int foo1 = 1;

    for (int i = 0; i < 100; i++) {
        int32_t data = generate_random_data();

        if (data > 0xFF) {
            foo1 = 0;
            return EXIT_FAILURE;
        }
    }
    return foo1;
}

// Vérifie que les octets 0 et 2 d'un tableau sont correctement inversés
int test_swap_be() {
    int foo2 = 1;
    uint8_t data[3];


    for (int i = 0; i < 3; i++) {
        data[i] = generate_random_data();
    }

    uint8_t original_data_0 = data[0];
    uint8_t original_data_2 = data[2];

    swap_be_8(data);

    if (data[0] == original_data_0 || data[2] == original_data_2) {
        foo2 = 0;
    }

    return foo2;
}

#ifdef TEST_BYTE_MAIN

// Fonction principale pour exécuter les tests liés à Byte.c
int main(void) {
    tst_t tests[] = {
            DECL_TEST(test_generate_random_data),
            DECL_TEST(test_swap_be),
            LAST_TEST
    };
    tst_all(tests);
    return EXIT_SUCCESS;
}
#endif
