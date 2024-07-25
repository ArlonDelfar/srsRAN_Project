#ifndef RLC_RX_UM_ENTITY_C_WRAPPER_H
#define RLC_RX_UM_ENTITY_C_WRAPPER_H

#include "rlc_rx_um_entity_c.h"

#ifdef __cplusplus
extern "C" {
#endif

void cpp_handle_um_pdu_outside_reassembly_window(rlc_rx_um_entity_t *entity, UMD_PDU *pdu);

#ifdef __cplusplus
}
#endif

#endif // RLC_RX_UM_ENTITY_C_WRAPPER_H
