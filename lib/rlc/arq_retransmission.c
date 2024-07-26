#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "arq_retransmission.h"

#define SN_SIZE_12_BIT 2
#define SN_FIELD_LENGTH_12_BIT 12

#define SN_SIZE_18_BIT 3
#define SN_FIELD_LENGTH_18_BIT 18

#define CFG_SN_SIZE SN_SIZE_12_BIT
#define CFG_SN_FIELD_LENGTH SN_FIELD_LENGTH_12_BIT
#define SO_SIZE 2

#define HEAD_MIN_SIZE CFG_SN_SIZE
#define HEAD_MAX_SIZE (CFG_SN_SIZE + SO_SIZE)

#define MIN(a,b) (((a) > (b)) ? (a) : (b))

enum rlc_si_field {
  full_sdu       = 0b00, ///< Full SDU
  first_segment  = 0b01, ///< First SDU segment
  last_segment   = 0b10, ///< Last SDU segment
  middle_segment = 0b11  ///< Middle SDU segment
};

enum lc_dc_field { 
  control = 0b00, 
  data = 0b01 
};

uint32_t get_retx_expected_hdr_len(struct rlc_tx_amd_retx* retx)
{
    return retx->so == 0 ? HEAD_MIN_SIZE : HEAD_MAX_SIZE;
}

bool get_polling_bit(void)
{
    return false;
}

size_t rlc_am_write_data_pdu_header(uint8_t* rlc_pdu_buf, struct rlc_am_pdu_header* hdr)
{
  uint8_t* rlc_pdu_buf_src_addr = rlc_pdu_buf;

  *rlc_pdu_buf = (hdr->dc & 0x01U) << 7U; // 1 bit D/C field

  *rlc_pdu_buf |= (hdr->p & 0x01U) << 6U;             // 1 bit P flag
  *rlc_pdu_buf |= (hdr->si & 0x03U) << 4U; // 2 bits SI

  if (hdr->sn_size == SN_FIELD_LENGTH_12_BIT) {
    // 12-bit SN
    *rlc_pdu_buf |= (hdr->sn >> 8U) & 0x0fU; // upper 4 bits of SN
    ++rlc_pdu_buf;
    *rlc_pdu_buf = hdr->sn & 0xffU; // remaining 8 bits of SN
  } else {
    // 18-bit SN
    *rlc_pdu_buf |= (hdr->sn >> 16U) & 0x3U; // upper 2 bits of SN
    ++rlc_pdu_buf;
    *rlc_pdu_buf = hdr->sn >> 8U; // center 8 bits of SN
    ++rlc_pdu_buf;
    *rlc_pdu_buf = hdr->sn & 0xffU; // lower 8 bits of SN
  }
  ++rlc_pdu_buf;

  if (hdr->so != 0) {
    // write SO
    *rlc_pdu_buf = hdr->so >> 8U; // upper part of SO
    ++rlc_pdu_buf;
    *rlc_pdu_buf = hdr->so & 0xffU; // lower part of SO
    ++rlc_pdu_buf;
  }
  return rlc_pdu_buf - rlc_pdu_buf_src_addr;
}

size_t arq_retransmission(uint8_t* rlc_pdu_buf, size_t rlc_pdu_buf_size, uint8_t* rlc_sdu_buf, size_t rlc_sdu_buf_size, struct rlc_tx_amd_retx* retx)
{
    if (rlc_pdu_buf == NULL || rlc_pdu_buf_size == NULL || rlc_sdu_buf == NULL || retx == NULL){
        return;
    }

    const size_t grant_len = rlc_pdu_buf_size;

    if (retx->length == 0)
    {
        return 0;
    }

    if (retx->so + retx->length > rlc_sdu_buf_size) {
        return 0;
    }

    uint32_t expected_hdr_len = get_retx_expected_hdr_len(retx);

    if (grant_len <= expected_hdr_len) {
        return 0;
    }

    size_t retx_payload_len = MIN(retx->length, grant_len - expected_hdr_len);
    bool retx_complete = retx_payload_len == retx->length;
    bool sdu_complete = retx->so + retx_payload_len >= rlc_sdu_buf_size;

    enum rlc_si_field si = full_sdu;
    if (retx->so == 0) {
        // either full SDU or first segment
        if (sdu_complete) {
        si = full_sdu;
        } else {
        si = first_segment;
        }
    } else {
        // either middle segment or last segment
        if (sdu_complete) {
        si = last_segment;
        } else {
        si = middle_segment;
        }
    }
  
    struct rlc_tx_amd_retx* retx_remainder = (struct rlc_tx_amd_retx* )calloc(0, sizeof(struct rlc_tx_amd_retx));
    if (!retx_complete) {
        // update SO and length of front element
        retx_remainder->so += retx_payload_len;
        retx_remainder->length -= retx_payload_len;
    }
    
    struct rlc_am_pdu_header hdr;
    hdr.dc                = data;
    hdr.p                 = get_polling_bit();
    hdr.si                = si;
    hdr.sn_size           = SN_FIELD_LENGTH_12_BIT;
    hdr.sn                = retx->sn;
    hdr.so                = retx->so;

    size_t header_len = rlc_am_write_data_pdu_header(rlc_pdu_buf, &hdr);

    if (header_len == 0) {
        return 0;
    }

    memcpy((rlc_pdu_buf + header_len), (rlc_sdu_buf + hdr.so), retx_payload_len);
    
    retx->length = retx_remainder->length;
    retx->so = retx_remainder->length;
    free(retx_remainder);

    return header_len + retx_payload_len;
}