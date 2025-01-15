#include "package.h"
#include "Test_package.h"
#include "Test_lib.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Vérifie la génération des données pour les voies et leur état
int test_generate_package() {
    ChannelData x = generate_channel_data(0x01); // Voie X
    ChannelData y = generate_channel_data(0x02); // Voie Y
    ChannelData z = generate_channel_data(0x03); // Voie Z
    Status status = generate_status();

    // Vérification des en-têtes
    int headers_correct = 1;
    if (x.header != 0x01 || y.header != 0x02 || z.header != 0x03 || status.header != 0x04) {
        headers_correct = 0;
    }

    // Vérification de la singularité des données entre les voies
    int data_different = 1;
    if ((x.data[0] == y.data[0] && x.data[1] == y.data[1] && x.data[2] == y.data[2]) ||
        (x.data[0] == z.data[0] && x.data[1] == z.data[1] && x.data[2] == z.data[2]) ||
        (y.data[0] == z.data[0] && y.data[1] == z.data[1] && y.data[2] == z.data[2])) {
        data_different = 0;
    }

    return (headers_correct && data_different) ? 1 : 0;
}

// Vérifie que les données des voies sont correctement inversées
int test_swap_channel_data() {
    int foo = 1;

    ChannelData x = {0x01, {generate_random_data(), generate_random_data(), generate_random_data()}};
    ChannelData y = {0x02, {generate_random_data(), generate_random_data(), generate_random_data()}};
    ChannelData z = {0x03, {generate_random_data(), generate_random_data(), generate_random_data()}};


    uint8_t x_original[3], y_original[3], z_original[3];
    memcpy(x_original, x.data, sizeof(x_original));
    memcpy(y_original, y.data, sizeof(y_original));
    memcpy(z_original, z.data, sizeof(z_original));


    swap_channel_data(&x, &y, &z);

    // Vérifie que les octets 0 et 2 ont été correctement inversés
    if (x.data[0] != x_original[2] || x.data[2] != x_original[0]) {
        foo = 0;
    }
    if (y.data[0] != y_original[2] || y.data[2] != y_original[0]) {
        foo = 0;
    }
    if (z.data[0] != z_original[2] || z.data[2] != z_original[0]) {
        foo = 0;
    }

    return foo;
}

#ifdef TEST_PACKAGE_MAIN

int main(void) {
    tst_t tests[] = {
        DECL_TEST(test_generate_package),
        DECL_TEST(test_swap_channel_data),
        LAST_TEST
    };
    tst_all(tests);
    return EXIT_SUCCESS;
}

#endif
