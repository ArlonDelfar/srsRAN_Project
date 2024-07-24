#ifndef RLC_RX_UM_ENTITY_EXPIRED_WRAPPER_H
#define RLC_RX_UM_ENTITY_EXPIRED_WRAPPER_H
#include <stdint.h>

extern "C" {
    #include "rlc_rx_um_entity_expired.h"
}

void wrapper_rlc_tx_um_entity_expired(struct rlc_rx_um_entity en);

struct rlc_rx_um_entity wrapper_rlc_tx_um_entity_init(
    uint32_t rx_next_reassembly, 
    uint32_t rx_timer_trigger, 
    uint32_t rx_next_highest, 
    uint32_t mod, 
    uint32_t um_window_size);

#endif