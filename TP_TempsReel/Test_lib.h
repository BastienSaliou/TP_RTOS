#ifndef TEST_LIB_H
#define TEST_LIB_H

#include <stdint.h>

#define DECL_TEST(tst) {&tst, #tst}
#define LAST_TEST {NULL,NULL}
#define HEADER_X 0x01  // Identifiant pour la voie X
#define HEADER_Y 0x02  // Identifiant pour la voie Y
#define HEADER_Z 0x03

typedef int(testfn_t)(void);

typedef struct tst_s {
    testfn_t *call;
    const char *name;
} tst_t;

void tst_all(tst_t *tst);

#endif