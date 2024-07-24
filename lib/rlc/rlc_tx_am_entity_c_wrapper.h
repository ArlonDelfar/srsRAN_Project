#ifndef RLC_TX_AM_ENTITY_C_WRAPPER_H
#define RLC_TX_AM_ENTITY_C_WRAPPER_H

extern "C" {
#include "rlc_tx_am_entity_c.h"
}

size_t wrapper_build_retx_pdu(rlc_tx_am_entity_t* entity, uint8_t* rlc_pdu_buf, size_t buf_size);



#endif // RLC_TX_AM_ENTITY_C_WRAPPER_H