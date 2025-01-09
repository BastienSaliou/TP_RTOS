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
    for (int i = 0; i < 3; i++) {
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

void generate_package() {
    ChannelData x = generate_channel_data(0x01); // Voie X
    ChannelData y = generate_channel_data(0x02); // Voie Y
    ChannelData z = generate_channel_data(0x03); // Voie Z

    Status status = generate_status();
}