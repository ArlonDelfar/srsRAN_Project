#include "rlc_rx_um_entity_expired.h"
#include <stdbool.h>

struct rlc_rx_um_entity rlc_tx_um_entity_init( uint32_t rx_next_reassembly, uint32_t rx_timer_trigger, uint32_t rx_next_highest, uint32_t mod, uint32_t um_window_size){
    

    return (struct rlc_rx_um_entity){
        .mod = mod,
        .um_window_size = um_window_size,
        .state = {
            .rx_next_reassembly = rx_next_reassembly,
            .rx_timer_trigger = rx_timer_trigger,
            .rx_next_highest = rx_next_highest
        }
    };
}
uint32_t rx_mod_base(uint32_t x, struct rlc_rx_um_entity en) { return (x - en.state.rx_next_highest - en.um_window_size) % en.mod; }

void rlc_tx_um_entity_expired(struct rlc_rx_um_entity en){
    // - if RX_Next_Highest > RX_Next_Reassembly + 1; or
    // - if RX_Next_Highest = RX_Next_Reassembly + 1 
    //   - start t-Reassembly;
    //   - set RX_Timer_Trigger to RX_Next_Highest.
    bool restart_reassembly_timer = false;
    if(rx_mod_base(en.state.rx_next_highest, en) > rx_mod_base(en.state.rx_next_reassembly + 1, en)){
        restart_reassembly_timer = true;
        en.state.rx_timer_trigger = en.state.rx_next_highest;
    }
    else if(rx_mod_base(en.state.rx_next_highest, en) == rx_mod_base(en.state.rx_next_reassembly + 1, en)){
        restart_reassembly_timer = true;
        en.state.rx_timer_trigger = en.state.rx_next_highest;
    }

}


