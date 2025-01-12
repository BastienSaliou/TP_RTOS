#include <stdio.h>
#include "Test_lib.h"
#include "color.h"

void tst_all(tst_t *tst) {
    int total_tests = 0;
    int passed_tests = 0;

    printf("Starting Tests...\n");

    for (; tst->call != NULL; tst++) {
        total_tests++;
        int ret = tst->call();

        const char *status = ret ? PRINT_BLUE("OK") : PRINT_RED("KO");
        if (ret) {
            passed_tests++;
        }
        printf("%s: %s\n", tst->name, status);
    }

    double percentage_passed = (double)passed_tests / total_tests * 100.0;
    double percentage_failed = 100.0 - percentage_passed;

    printf("\n--- Test Summary ---\n");
    printf("Total Tests: %d\n", total_tests);
    printf("Passed: %d (%.2f%%)\n", passed_tests, percentage_passed);
    printf("Failed: %d (%.2f%%)\n", total_tests - passed_tests, percentage_failed);
    printf("--------------------\n");
    printf("All test completed\n");
}
