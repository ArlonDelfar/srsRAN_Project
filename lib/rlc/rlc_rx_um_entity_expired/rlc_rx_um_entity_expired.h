#ifndef RLC_RX_UM_ENTITY_EXPIRED_H
#define RLC_RX_UM_ENTITY_EXPIRED_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

struct rlc_rx_um_state {
  uint32_t rx_next_reassembly;
  uint32_t rx_timer_trigger;
  uint32_t rx_next_highest;
};

struct rlc_rx_um_entity{
    int32_t mod;
    uint32_t um_window_size;
    struct rlc_rx_um_state state;
};

void rlc_tx_um_entity_expired(struct rlc_rx_um_entity en);
struct rlc_rx_um_entity rlc_tx_um_entity_init(
  uint32_t rx_next_reassembly, 
  uint32_t rx_timer_trigger, 
  uint32_t rx_next_highest, 
  uint32_t mod, 
  uint32_t um_window_size);

#endif