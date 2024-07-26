#ifndef ARQ_RETRANSMISSION_WRAPPER_H
#define ARQ_RETRANSMISSION_WRAPPER_H

extern "C" {
#include "arq_retransmission.h"
}

void arq_retransmission_wrapper(uint8_t* rlc_pdu_buf, 
                                size_t rlc_pdu_buf_size, 
                                uint8_t* rlc_sdu_buf,
                                size_t rlc_sdu_buf_size,
                                struct rlc_tx_amd_retx* retx);

#endif //ARQ_RETRANSMISSION_WRAPPER_H