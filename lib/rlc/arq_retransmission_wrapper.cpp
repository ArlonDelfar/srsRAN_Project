#include "arq_retransmission_wrapper.h"
#include "rlc_retx_queue.h"

rlc_tx_amd_retx* create_rlc_tx_amd_retx_c(struct srsran::rlc_tx_amd_retx* retx_cpp_ptr)
{
    rlc_tx_amd_retx* retx_c_ptr = (rlc_tx_amd_retx* )malloc(sizeof(struct rlc_tx_amd_retx));
    retx_c_ptr->sn = retx_cpp_ptr->sn;
    retx_c_ptr->so = retx_cpp_ptr->so;
    retx_c_ptr->length = retx_cpp_ptr->length;

    return retx_c_ptr;
}

void set_rlc_tx_amd_retx_cpp(struct srsran::rlc_tx_amd_retx* retx_cpp_ptr, struct rlc_tx_amd_retx* retx_c_ptr)
{
    retx_cpp_ptr->sn = retx_c_ptr->sn;
    retx_cpp_ptr->so = retx_c_ptr->so;
    retx_cpp_ptr->length = retx_c_ptr->length;

    free(retx_c_ptr);
}

void arq_retransmission_wrapper(uint8_t* rlc_pdu_buf, size_t rlc_pdu_buf_size, uint8_t* rlc_sdu_buf, size_t rlc_sdu_buf_size, struct srsran::rlc_tx_amd_retx* retx)
{
    rlc_tx_amd_retx* retx_c_ptr = create_rlc_tx_amd_retx_c(retx);
    arq_retransmission(rlc_pdu_buf, rlc_pdu_buf_size, rlc_sdu_buf, rlc_sdu_buf_size, retx_c_ptr);
    set_rlc_tx_amd_retx_cpp(retx, retx_c_ptr);
}