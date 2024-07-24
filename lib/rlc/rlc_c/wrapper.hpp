#ifndef WRAPPER_H
#define WRAPPER_H

extern "C" {
#include "rlc_52221.h"
}

bool sn_in_reassembly_window(const uint32_t sn, struct rlc_rx_um_entity* entity);
   
#endif 