#include <stdio.h>
#include <stdlib.h> // Pour EXIT_SUCCESS
#include "Test_lib.h"
#include "Byte.h"

#define DECL_TEST(tst) {&tst, #tst}
#define LAST_TEST {NULL,NULL}

typedef int(testfn_t)(void);

typedef struct tst_s {
    testfn_t *call;
    const char *name;
} tst_t;


void tst_all(tst_t *tst) {
    for (; tst->call != NULL; tst++) {
        int ret = tst->call();
        const char *status = ret ? "OK" : "KO";
        printf("%s: %s\n", tst->name, status);
    }
}


int pass(void) {
    return 1;
}

int fail(void) {
    return 0;
}


int main(void) {

    tst_t tests[] = {
            DECL_TEST(pass),
            DECL_TEST(fail),
            DECL_TEST(test_generate_random_data),
            DECL_TEST(test_swap_be),
            LAST_TEST //Sentinelle
    };

    printf("Starting Tests...\n");
    tst_all(tests);
    printf("All Tests Completed.\n");

    return EXIT_SUCCESS;
}
