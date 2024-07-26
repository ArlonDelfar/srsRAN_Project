#ifndef ARQ_RETRANSMISSION_H
#define ARQ_RETRANSMISSION_H
#include <stdint.h>

typedef struct {
  uint32_t sn;
  uint32_t so;
  uint32_t length;
} rlc_tx_amd_retx_t;

struct rlc_am_pdu_header {
  uint8_t   dc;      ///< Data/Control (D/C) field
  uint8_t   p;       ///< Polling bit
  uint8_t   si;      ///< Segmentation info
  uint8_t   sn_size; ///< Sequence number size (12 or 18 bits)
  uint32_t  sn;      ///< Sequence number
  uint16_t  so;      ///< Sequence offset
};

struct rlc_tx_amd_retx {
  uint32_t sn;
  uint32_t so;
  uint32_t length;
};

size_t arq_retransmission(uint8_t* rlc_pdu_buf, size_t rlc_pdu_buf_size, uint8_t* rlc_sdu_buf, size_t rlc_sdu_buf_size,  struct rlc_tx_amd_retx* retx);

#endif //ARQ_RETRANSMISSION_H