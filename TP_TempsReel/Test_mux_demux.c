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


int test_shuffle() {
    int order[4] = {0, 1, 2, 3};
    int foo6 = 1;

    for (int n = 0; n < 5; n++) {
        int original[4];
        memcpy(original, order, sizeof(order));

        shuffle(order, 5);

        if (memcmp(original, order, sizeof(order)) == 0) {
            foo6 = 0;
        }
        return foo6;
    }
}


int test_multiplex() {
    ChannelData x = generate_channel_data(0x01);
    ChannelData y = generate_channel_data(0x02);
    ChannelData z = generate_channel_data(0x03);
    Status status = generate_status();

    uint8_t trames[NUM_CALLS][TRAME_SIZE];
    int foo4 = 0;

    // teste 5 trames
    for (int i = 0; i < NUM_CALLS; i++) {
        multiplex(trames[i], x, y, z, status);
    }

    for (int i = 0; i < NUM_CALLS - 1; i++) {
        if (memcmp(trames[i], trames[i + 1], TRAME_SIZE) != 0) {
            foo4 = 1;
            break;
        }
    }
    return foo4;
}
int test_demultiplex() {
    ChannelData x_original = generate_channel_data(0x01); // Voie X
    ChannelData y_original = generate_channel_data(0x02); // Voie Y
    ChannelData z_original = generate_channel_data(0x03); // Voie Z
    Status status_original = generate_status();

    uint8_t trame[TRAME_SIZE];

    multiplex(trame, x_original, y_original, z_original, status_original);
    /*printf("Trame multiplex : ");
    for (int i = 0; i < TRAME_SIZE; i++) {
        printf("%02X ", trame[i]);
    }
    printf("\n");*/
    ChannelData x_demux = {0}, y_demux = {0}, z_demux = {0};
    Status status_demux = {0};

    demultiplex(trame, &x_demux, &y_demux, &z_demux, &status_demux);

    /*printf("X : %02X %02X %02X %02X\n", x_demux.header, x_demux.data[0], x_demux.data[1], x_demux.data[2]);
    printf("Y : %02X %02X %02X %02X\n", y_demux.header, y_demux.data[0], y_demux.data[1], y_demux.data[2]);
    printf("Z : %02X %02X %02X %02X\n", z_demux.header, z_demux.data[0], z_demux.data[1], z_demux.data[2]);
    printf("Status : %02X %02X\n", status_demux.header, status_demux.status);*/

    swap_channel_data(&x_demux, &y_demux, &z_demux);

    /*printf("X : %02X %02X %02X %02X\n", x_demux.header, x_demux.data[0], x_demux.data[1], x_demux.data[2]);
    printf("Y : %02X %02X %02X %02X\n", y_demux.header, y_demux.data[0], y_demux.data[1], y_demux.data[2]);
    printf("Z : %02X %02X %02X %02X\n", z_demux.header, z_demux.data[0], z_demux.data[1], z_demux.data[2]);*/

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
            LAST_TEST //Sentinelle
    };

    tst_all(tests);
    return EXIT_SUCCESS;
}
#endif
