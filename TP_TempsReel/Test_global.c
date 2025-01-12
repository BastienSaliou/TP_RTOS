#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Test_lib.h"
#include "Test_byte.h"
#include "Test_mux_demux.h"
#include "Test_package.h"

int main(void) {
    srand(time(NULL));
    tst_t tests[] = {
            DECL_TEST(test_generate_random_data),
            DECL_TEST(test_swap_be),
            DECL_TEST(test_multiplex),
            DECL_TEST(test_demultiplex),
            DECL_TEST(test_generate_package),
            DECL_TEST(test_shuffle),
            LAST_TEST
    };

    tst_all(tests);
    return EXIT_SUCCESS;
}
