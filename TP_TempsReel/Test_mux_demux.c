#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stdlib.h"
#include "package.h"
#include "mux_demux.h"
#include "Test_mux_demux.h"
#include "Test_lib.h"



int test_multiplex() {

    ChannelData x = generate_channel_data(0x01); // Voie X
    ChannelData y = generate_channel_data(0x02); // Voie Y
    ChannelData z = generate_channel_data(0x03); // Voie Z
    Status status = generate_status();

    uint8_t trame[TRAME_SIZE];

    multiplex(trame, x, y, z, status);

    /*for (int i = 0; i < TRAME_SIZE; i++) {
        printf("%02X ", trame[i]);
    }
    printf("\n");*/

    int foo4 = 1;
    int j = 0;

    //voie X
    if (trame[j++] != x.header)
        foo4 = 0;
    if (memcmp(&trame[j], x.data, sizeof(x.data)) != 0)
        foo4 = 0;
    j += sizeof(x.data);

    //voie Y
    if (trame[j++] != y.header)
        foo4 = 0;
    if (memcmp(&trame[j], y.data, sizeof(y.data)) != 0)
        foo4 = 0;
    j += sizeof(y.data);

    //voie Z
    if (trame[j++] != z.header)
        foo4 = 0;
    if (memcmp(&trame[j], z.data, sizeof(z.data)) != 0)
        foo4 = 0;
    j += sizeof(z.data);

    if (trame[j++] != status.header)
        foo4 = 0;
    if (trame[j++] != status.status)
        foo4 = 0;

    return foo4;
}
int test_demultiplex() {
    ChannelData x_original = generate_channel_data(0x01); // Voie X
    ChannelData y_original = generate_channel_data(0x02); // Voie Y
    ChannelData z_original = generate_channel_data(0x03); // Voie Z
    Status status_original = generate_status();

    uint8_t trame[TRAME_SIZE];

    multiplex(trame, x_original, y_original, z_original, status_original);

    /*for (int i = 0; i < TRAME_SIZE; i++) {
        printf("%02X ", trame[i]);
    }
    printf("\n");*/

    ChannelData x_demux, y_demux, z_demux;
    Status status_demux;

    demultiplex(trame, &x_demux, &y_demux, &z_demux, &status_demux);

    /*printf("Voie X : %02X %02X %02X %02X\n",
           x_demux.header, x_demux.data[0], x_demux.data[1], x_demux.data[2]);
    printf("Voie Y : %02X %02X %02X %02X\n",
           y_demux.header, y_demux.data[0], y_demux.data[1], y_demux.data[2]);
    printf("Voie Z : %02X %02X %02X %02X\n",
           z_demux.header, z_demux.data[0], z_demux.data[1], z_demux.data[2]);
    printf("Status : %02X %02X\n",
           status_demux.header, status_demux.status);*/

    int foo5 = 1;

    //voie X
    if (x_demux.header != x_original.header ||
        memcmp(x_demux.data, x_original.data, sizeof(x_original.data)) != 0) {
        foo5 = 0;

    }

    //voie Y
    if (y_demux.header != y_original.header ||
        memcmp(y_demux.data, y_original.data, sizeof(y_original.data)) != 0) {
        foo5 = 0;
    }

    //voie Z
    if (z_demux.header != z_original.header ||
        memcmp(z_demux.data, z_original.data, sizeof(z_original.data)) != 0) {
        foo5 = 0;
    }

    //Status
    if (status_demux.header != status_original.header ||
        status_demux.status != status_original.status) {
        foo5 = 0;

    }
    return foo5;

}
#ifdef TEST_MUX_DEMUX_MAIN
int main(void) {

    tst_t tests[] = {
            DECL_TEST(test_multiplex),
            DECL_TEST(test_demultiplex),
            LAST_TEST //Sentinelle
    };

    tst_all(tests);
    return EXIT_SUCCESS;
}
#endif

