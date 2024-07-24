#include "rlc_rx_um_entity_expired_wrapper.h"

void wrapper_rlc_tx_um_entity_expired(struct rlc_rx_um_entity en) {
    rlc_tx_um_entity_expired(en);
}

struct rlc_rx_um_entity wrapper_rlc_tx_um_entity_init(uint32_t rx_next_reassembly, 
uint32_t rx_timer_trigger, uint32_t rx_next_highest, uint32_t mod, uint32_t um_window_size) {
    return rlc_tx_um_entity_init(rx_next_reassembly, rx_timer_trigger, rx_next_highest, mod, um_window_size);
}
