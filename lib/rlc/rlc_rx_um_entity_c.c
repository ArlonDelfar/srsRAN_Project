#include "rlc_rx_um_entity_c_wrapper.h"
#include "rlc_rx_um_entity_c.h"
#include <stdio.h>
#include <stdlib.h>

void handle_pdu(rlc_rx_um_entity_t* entity, uint8_t* buf, size_t length) {
    rlc_rx_um_entity_handle_pdu(entity, buf, length);
}

void check_reassembly_timer(rlc_rx_um_entity_t* entity) {
    rlc_rx_um_entity_check_reassembly_timer(entity);
}
