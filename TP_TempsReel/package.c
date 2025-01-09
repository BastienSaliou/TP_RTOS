#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    uint8_t channel;
    uint32_t data;
} ChannelData;


typedef struct {
    uint8_t header;
    uint8_t status;
} Status;

ChannelData generate_channel_data(uint8_t){
    ChannelData ch;


}



