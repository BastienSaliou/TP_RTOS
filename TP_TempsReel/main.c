#include "test_lib.h"
#include <stdio.h>

const char * strbool(int v){
    static const char* array[2] = {"FAIL" , "PASSED"};
    return array[v & 1];
}
int main(void) {
    printf("Starting Tests...\n");

    printf(" Test : %s\n", strbool(test1()));
    printf(" Test : %s\n", strbool(test2()));
    printf(" Test : %s\n", strbool(test3()));
    printf(" Test : %s\n", strbool(test4()));

    printf("All Tests Completed.\n");
    return 0;
}
