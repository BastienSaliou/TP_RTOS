#include "package.h"
#include "Test_package.h"
#include "Test_lib.h"
#include <stdint.h>
#include <stdlib.h>


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

#ifdef TEST_PACKAGE_MAIN
int main(void) {

    tst_t tests[] = {
            DECL_TEST(test_generate_package),
            LAST_TEST //Sentinelle
    };
    tst_all(tests);
    return EXIT_SUCCESS;
}
#endif