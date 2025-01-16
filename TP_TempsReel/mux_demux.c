#include <string.h>
#include <stdlib.h>
#include "package.h"
#include "Byte.h"

#define TRAME_SIZE 14

// Mélange aléatoire d'un tableau d'entiers
void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand8() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void multiplex(uint8_t *trame, ChannelData x, ChannelData y, ChannelData z, Status status) {
    int i = 0;
    int order[4] = {0, 1, 2, 3};
    shuffle(order, 4); // Mélange aléatoire des éléments

    // Ajout des données dans la trame selon l'ordre aléatoire
    for (int k = 0; k < 4; k++) {
        switch (order[k]) {
            case 0:
                trame[i++] = x.header;
                memcpy(&trame[i], x.data, sizeof(x.data));
                i += sizeof(x.data);
                break;
            case 1:
                trame[i++] = y.header;
                memcpy(&trame[i], y.data, sizeof(y.data));
                i += sizeof(y.data);
                break;
            case 2:
                trame[i++] = z.header;
                memcpy(&trame[i], z.data, sizeof(z.data));
                i += sizeof(z.data);
                break;
            case 3:
                trame[i++] = status.header;
                trame[i++] = status.status;
                break;
        }
    }
}

void demultiplex(uint8_t *trame, ChannelData *x, ChannelData *y, ChannelData *z, Status *status) {
    int i = 0;

    // Parcours de la trame pour identifier et extraire chaque élément
    while (i < TRAME_SIZE) {
        uint8_t header = trame[i++];
        switch (header) {
            case 0x01:
                x->header = header;
                memcpy(x->data, &trame[i], sizeof(x->data));
                i += sizeof(x->data);
                break;

            case 0x02:
                y->header = header;
                memcpy(y->data, &trame[i], sizeof(y->data));
                i += sizeof(y->data);
                break;

            case 0x03:
                z->header = header;
                memcpy(z->data, &trame[i], sizeof(z->data));
                i += sizeof(z->data);
                break;

            case 0x04:
                status->header = header;
                status->status = trame[i++];
                break;
        }
    }
}
