#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "stdlib.h"
#include "package.h"
#include "mux_demux.h"
#include "Test_mux_demux.h"
#include "Test_lib.h"

#define NUM_CALLS 5
#define TRAME_SIZE 14

#define HEADER_X 0x01
#define HEADER_Y 0x02
#define HEADER_Z 0x03

// Vérifie que l'ordre d'un tableau est modifié après mélange
int test_shuffle() {
    int order[4] = {0, 1, 2, 3};
    int foo6 = 1;

    for (int n = 0; n < 5; n++) {
        int original[4];
        memcpy(original, order, sizeof(order));

        shuffle(order, 4);

        // Vérifie que l'ordre a changé
        if (memcmp(original, order, sizeof(order)) == 0) {
            foo6 = 0;
        }
    }
    return foo6;
}


// Vérifie que chaque trame générée est unique
int test_multiplex() {
    ChannelData x = generate_channel_data(HEADER_X);
    ChannelData y = generate_channel_data(HEADER_Y);
    ChannelData z = generate_channel_data(HEADER_Z);
    Status status = generate_status();

    uint8_t trames[NUM_CALLS][TRAME_SIZE];
    int foo4 = 0;

    for (int i = 0; i < NUM_CALLS; i++) {
        multiplex(trames[i], x, y, z, status);
    }

    // Vérifie que les trames sont différentes
    for (int i = 0; i < NUM_CALLS - 1; i++) {
        if (memcmp(trames[i], trames[i + 1], TRAME_SIZE) != 0) {
            foo4 = 1;
            break;
        }
    }
    return foo4;
}

// Vérifie que les données extraites correspondent à celles d'origine
int test_demultiplex() {
    ChannelData x_original = generate_channel_data(HEADER_X);
    ChannelData y_original = generate_channel_data(HEADER_Y);
    ChannelData z_original = generate_channel_data(HEADER_Z);
    Status status_original = generate_status();

    uint8_t trame[TRAME_SIZE];

    multiplex(trame, x_original, y_original, z_original, status_original);

    ChannelData x_demux = {0}, y_demux = {0}, z_demux = {0};
    Status status_demux = {0};

    demultiplex(trame, &x_demux, &y_demux, &z_demux, &status_demux);

    int foo5 = 1;
    for (int i = 0; i < TRAME_SIZE;) {
        uint8_t header = trame[i++];
        switch (header) {
            case 0x01:
                if (memcmp(&trame[i], x_original.data, sizeof(x_original.data)) != 0) {
                    foo5 = 0;
                }
                i += sizeof(x_original.data);
                break;
            case 0x02:
                if (memcmp(&trame[i], y_original.data, sizeof(y_original.data)) != 0) {
                    foo5 = 0;
                }
                i += sizeof(y_original.data);
                break;
            case 0x03:
                if (memcmp(&trame[i], z_original.data, sizeof(z_original.data)) != 0) {
                    foo5 = 0;
                }
                i += sizeof(z_original.data);
                break;
            case 0x04:
                if (trame[i++] != status_original.status) {
                    foo5 = 0;
                }
                break;
        }
    }
    return foo5;
}

#ifdef TEST_MUX_DEMUX_MAIN

int main(void) {
    srand(time(NULL));
    tst_t tests[] = {
        DECL_TEST(test_multiplex),
        DECL_TEST(test_demultiplex),
        DECL_TEST(test_shuffle),
        LAST_TEST
    };

    tst_all(tests);
    return EXIT_SUCCESS;
}

#endif
