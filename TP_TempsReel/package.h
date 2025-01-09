#ifndef PACKAGE_H
#define PACKAGE_H

#include <stdint.h>
#include "Byte.h"


typedef struct {
    uint8_t header;
    uint8_t data[3];
} ChannelData;


typedef struct {
    uint8_t header;
    uint8_t status;
} Status;


void generate_channel_data_random(uint8_t *data);
ChannelData generate_channel_data(uint8_t header);
Status generate_status();
void generate_package();


#endif