#ifndef MUX_DEMUX_H
#define MUX_DEMUX_H

#include <stdint.h>
#include "package.h"

void multiplex(uint8_t *trame, ChannelData x, ChannelData y, ChannelData z, Status status);
void demultiplex(uint8_t *trame, ChannelData *x, ChannelData *y, ChannelData *z, Status *status);
void shuffle(int *array, int n);

#endif
