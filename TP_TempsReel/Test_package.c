#include "package.h"
#include "Test_package.h"
#include "Test_lib.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


int test_generate_package() {
    ChannelData x = generate_channel_data(0x01); // Voie X
    ChannelData y = generate_channel_data(0x02); // Voie Y
    ChannelData z = generate_channel_data(0x03); // Voie Z
    Status status = generate_status();

    int headers_correct = 1;
    if (x.header != 0x01) {
        headers_correct = 0;
    }
    if (y.header != 0x02) {
        headers_correct = 0;
    }
    if (z.header != 0x03) {
        headers_correct = 0;
    }
    if (status.header != 0x04) {
        headers_correct = 0;
    }

    int data_different = 1;
    if ((x.data[0] == y.data[0] && x.data[1] == y.data[1] && x.data[2] == y.data[2]) ||
        (x.data[0] == z.data[0] && x.data[1] == z.data[1] && x.data[2] == z.data[2]) ||
        (y.data[0] == z.data[0] && y.data[1] == z.data[1] && y.data[2] == z.data[2])) {
        data_different = 0;
    }
    int foo3 = 1;
    if (data_different && headers_correct){
        return foo3;
    }else{
        foo3 = 0;
        return foo3;
    }
}

int test_swap_channel_data() {
    int foo = 1;

    ChannelData x = {0x01, {generate_random_data(), generate_random_data(), generate_random_data()}};
    ChannelData y = {0x02, {generate_random_data(), generate_random_data(), generate_random_data()}};
    ChannelData z = {0x03, {generate_random_data(), generate_random_data(), generate_random_data()}};

    uint8_t x_original[3], y_original[3], z_original[3];
    memcpy(x_original, x.data, sizeof(x_original));
    memcpy(y_original, y.data, sizeof(y_original));
    memcpy(z_original, z.data, sizeof(z_original));

    /*printf("Avant inversion :\n");
    printf("X : %02X %02X %02X %02X\n", x.header, x.data[0], x.data[1], x.data[2]);
    printf("Y : %02X %02X %02X %02X\n", y.header, y.data[0], y.data[1], y.data[2]);
    printf("Z : %02X %02X %02X %02X\n", z.header, z.data[0], z.data[1], z.data[2]);*/

    swap_channel_data(&x, &y, &z);

    /*printf("Apres inversion :\n");
    printf("X : %02X %02X %02X %02X\n", x.header, x.data[0], x.data[1], x.data[2]);
    printf("Y : %02X %02X %02X %02X\n", y.header, y.data[0], y.data[1], y.data[2]);
    printf("Z : %02X %02X %02X %02X\n", z.header, z.data[0], z.data[1], z.data[2]);*/

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
            LAST_TEST //Sentinelle
    };
    tst_all(tests);
    return EXIT_SUCCESS;
}
#endif