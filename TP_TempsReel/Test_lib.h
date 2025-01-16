#ifndef TEST_LIB_H
#define TEST_LIB_H

#include <stdint.h>

#define DECL_TEST(tst) {&tst, #tst}
#define LAST_TEST {NULL,NULL}

typedef int(testfn_t)(void);

typedef struct tst_s {
    testfn_t *call;
    const char *name;
} tst_t;

void tst_all(tst_t *tst);

#endif