#include "Byte.h"

typedef struct {
    uint8_t header;
    uint8_t data[3];
} ChannelData;


typedef struct {
    uint8_t header;
    uint8_t status;
} Status;


void generate_channel_data_random(uint8_t *data) {
		int i=0;
    for (i = 0; i < 3; i++) {
        data[i] = generate_random_data();
    }
}


ChannelData generate_channel_data(uint8_t header) {
    ChannelData channel;
    channel.header = header;
    generate_channel_data_random(channel.data);
    return channel;
}


Status generate_status() {
    Status status;
    status.header = 0x04;
    status.status = 0x00;
    return status;
}

void swap_channel_data(ChannelData *x, ChannelData *y, ChannelData *z) {
    if (x) {
        swap_be_8(x->data);
    }
    if (y) {
        swap_be_8(y->data);
    }
    if (z) {
        swap_be_8(z->data);
    }
}