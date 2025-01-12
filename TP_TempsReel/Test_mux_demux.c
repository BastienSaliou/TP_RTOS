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

    for (int i = 0; i < TRAME_SIZE; i++) {
        printf("%02X ", trame[i]);
    }
    printf("\n");

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

int main(void) {

    tst_t tests[] = {
            DECL_TEST(test_multiplex),
            LAST_TEST //Sentinelle
    };

    printf("Starting Tests...\n");
    tst_all(tests);
    printf("All Tests Completed.\n");

    return EXIT_SUCCESS;
}


