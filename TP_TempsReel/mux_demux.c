#include <string.h>
#include "package.h"


void multiplex(uint8_t *trame, ChannelData x, ChannelData y, ChannelData z, Status status) {
    int i = 0;

    // Ajouter la voie X
    trame[i++] = x.header;
    memcpy(&trame[i], x.data, sizeof(x.data));
    i += sizeof(x.data);

    // Ajouter la voie Y
    trame[i++] = y.header;
    memcpy(&trame[i], y.data, sizeof(y.data));
    i += sizeof(y.data);

    // Ajouter la voie Z
    trame[i++] = z.header;
    memcpy(&trame[i], z.data, sizeof(z.data));
    i += sizeof(z.data);

    // Ajouter la voie Status
    trame[i++] = status.header;
    trame[i++] = status.status;
}

void demultiplex(uint8_t *trame, ChannelData *x, ChannelData *y, ChannelData *z, Status *status) {
    int i = 0;

    // Extraire la voie X
    x->header = trame[i++];
    memcpy(x->data, &trame[i], sizeof(x->data));
    i += sizeof(x->data);

    // Extraire la voie Y
    y->header = trame[i++];
    memcpy(y->data, &trame[i], sizeof(y->data));
    i += sizeof(y->data);

    // Extraire la voie Z
    z->header = trame[i++];
    memcpy(z->data, &trame[i], sizeof(z->data));
    i += sizeof(z->data);

    // Extraire la voie Status
    status->header = trame[i++];
    status->status = trame[i++];
}

