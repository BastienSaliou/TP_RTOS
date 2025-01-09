#ifndef PACKAGE_H
#define PACKAGE_H

#include <stdint.h>
#include "Byte.h"


typedef struct {
    uint8_t header;
    uint32_t data[3];
} ChannelData;


typedef struct {
    uint8_t header;
    uint8_t status;
} Status;

void print_channel_data(const ChannelData *channe0);
void print_acquisition_status(const Status *status);
void generate_package();
void test_generate_package();



#endif