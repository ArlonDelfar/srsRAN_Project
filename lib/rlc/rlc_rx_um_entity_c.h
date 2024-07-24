#ifndef RLC_RX_UM_ENTITY_C_H
#define RLC_RX_UM_ENTITY_C_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct rlc_rx_um_entity_t rlc_rx_um_entity_t;

rlc_rx_um_entity_t* rlc_rx_um_entity_create(
        int gnb_du_id,
        int ue_index,
        int rb_id,
        int config_sn_field_length,
        int config_t_reassembly,
        bool metrics_enabled);
        
    void rlc_rx_um_entity_handle_pdu(rlc_rx_um_entity_t* entity, uint8_t* data, size_t length);

    void rlc_rx_um_entity_check_reassembly_timer(rlc_rx_um_entity_t* entity);

#endif // RLC_RX_UM_ENTITY_C_H