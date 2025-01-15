#include "Byte.h"

// Structure représentant une voie avec un en-tête et 3 octets de données
typedef struct {
    uint8_t header;
    uint8_t data[3];
} ChannelData;

typedef struct {
    uint8_t header;
    uint8_t status;
} Status;

// Génère des données aléatoires pour une voie
void generate_channel_data_random(uint8_t *data) {
    for (int i = 0; i < 3; i++) {
        data[i] = generate_random_data();
    }
}

// Crée une structure ChannelData avec un en-tête donné et des données aléatoires
ChannelData generate_channel_data(uint8_t header) {
    ChannelData channel;
    channel.header = header;
    generate_channel_data_random(channel.data); // Remplit les données aléatoires
    return channel;
}

Status generate_status() {
    Status status;
    status.header = 0x04;
    status.status = 0x00;
    return status;
}

// Inverse les octets des données pour les structures ChannelData en big-endian
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
