#include <stdio.h>
#include <stdlib.h> // Pour EXIT_SUCCESS
#include "Test_lib.h"





void tst_all(tst_t *tst) {
    for (; tst->call != NULL; tst++) {
        int ret = tst->call();
        const char *status = ret ? "OK" : "KO";
        printf("%s: %s\n", tst->name, status);
    }
}

