#ifndef BYTE_H
#define BYTE_H

#include <stdint.h>

uint32_t generate_random_data();
int test_generate_random_data();
uint32_t swap_be(uint32_t data);
int test_swap_be();

#endif
