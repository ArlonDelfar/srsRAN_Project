#include "rlc_tx_am_entity_c_wrapper.h"

size_t wrapper_build_retx_pdu(rlc_tx_am_entity_t* entity, uint8_t* rlc_pdu_buf, size_t buf_size)
{
    return build_retx_pdu(entity, rlc_pdu_buf, buf_size);
}