#ifndef TEST_MUX_DEMUX_H
#define TEST_MUX_DEMUX_H

#include <stdint.h>
#include "mux_demux.h"

int test_multiplex();
int test_demultiplex();
int test_shuffle();
void swap_second_and_fourth(uint8_t *data);
void apply_swap_to_demultiplexed_data(ChannelData *x, ChannelData *y, ChannelData *z);


#endif