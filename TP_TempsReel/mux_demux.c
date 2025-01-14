#include <string.h>
#include <stdlib.h>
#include "package.h"


void shuffle(int *array, int n) {
		int i = 0;
    for (i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}



void multiplex(uint8_t *trame, ChannelData x, ChannelData y, ChannelData z, Status status) {
    int i = 0;
		int k = 0;
    int order[4] = {0, 1, 2, 3};
    shuffle(order, 4);

    for (k = 0; k < 4; k++) {
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

    x->header = trame[i++];
    memcpy(x->data, &trame[i], sizeof(x->data));
    i += sizeof(x->data);

    y->header = trame[i++];
    memcpy(y->data, &trame[i], sizeof(y->data));
    i += sizeof(y->data);

    z->header = trame[i++];
    memcpy(z->data, &trame[i], sizeof(z->data));
    i += sizeof(z->data);

    status->header = trame[i++];
    status->status = trame[i++];
}

