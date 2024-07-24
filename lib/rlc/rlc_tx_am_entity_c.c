#include "rlc_tx_am_entity_c.h"
#include <stdio.h>

size_t build_retx_pdu(rlc_tx_am_entity_t* entity, uint8_t* rlc_pdu_buf, size_t buf_size)
{
    const size_t grant_len = buf_size;

    if (entity->retx_queue->empty()) {
        printf("Error: Called build_retx_pdu() but retx_queue is empty.\n");
        return 0;
    }

    while (!entity->tx_window->has_sn(entity->tx_window, entity->retx_queue->front().sn)) {
        printf("Info: Could not find sn=%u in tx window, dropping RETX.\n", entity->retx_queue->front().sn);
        entity->retx_queue->pop();
        if (entity->retx_queue->empty()) {
            printf("Info: Empty retx_queue, cannot provide any PDU for retransmission.\n");
            return 0;
        }
    }

    const rlc_tx_amd_retx retx = entity->retx_queue->front();
    printf("Debug: Processing RETX. sn=%u so=%zu length=%zu\n", retx.sn, retx.so, retx.length);
    entity->retx_sn = retx.sn;

    rlc_tx_am_sdu_info* sdu_info = entity->tx_window->get_sdu_info(entity->tx_window, retx.sn);

    if (retx.so + retx.length > sdu_info->sdu.length) {
        printf("Error: Skipping invalid RETX that exceeds SDU boundaries. sn=%u so=%zu length=%zu sdu_len=%zu grant_len=%zu\n",
                     retx.sn, retx.so, retx.length, sdu_info->sdu.length, grant_len);
        entity->retx_queue->pop();
        return 0;
    }

    size_t expected_hdr_len = entity->get_retx_expected_hdr_len(retx);
    if (grant_len <= expected_hdr_len) {
        printf("Debug: Cannot fit RETX SDU into grant_len=%zu. expected_hdr_len=%zu\n", grant_len, expected_hdr_len);
        return 0;
    }

    size_t retx_payload_len = retx.length < (grant_len - expected_hdr_len) ? retx.length : (grant_len - expected_hdr_len);
    bool retx_complete = retx_payload_len == retx.length;
    bool sdu_complete = retx.so + retx_payload_len >= sdu_info->sdu.length;

    rlc_si_field si = rlc_si_field_full_sdu;
    if (retx.so == 0) {
        si = sdu_complete ? rlc_si_field_full_sdu : rlc_si_field_first_segment;
    } else {
        si = sdu_complete ? rlc_si_field_last_segment : rlc_si_field_middle_segment;
    }

    printf("Debug: Creating RETX PDU. sn=%u si=%d retx_payload_len=%zu expected_hdr_len=%zu grant_len=%zu\n",
                   retx.sn, si, retx_payload_len, expected_hdr_len, grant_len);

    if (retx_complete) {
        entity->retx_queue->pop();
    } else {
        rlc_tx_amd_retx retx_remainder = entity->retx_queue->front();
        retx_remainder.so += retx_payload_len;
        retx_remainder.length -= retx_payload_len;
        entity->retx_queue->replace_front(retx_remainder);
    }

    rlc_am_pdu_header hdr = {0};
    hdr.dc = rlc_dc_field_data;
    hdr.p = entity->get_polling_bit(retx.sn, true, 0);
    hdr.si = si;
    hdr.sn_size = entity->cfg.sn_field_length;
    hdr.sn = retx.sn;
    hdr.so = retx.so;

    size_t header_len = entity->rlc_am_write_data_pdu_header(rlc_pdu_buf, hdr);
    if (header_len == 0) {
        printf("Error: Could not pack RLC header. sn=%u\n", hdr.sn);
        return 0;
    }

    size_t payload_len = entity->copy_segments(sdu_info->sdu.data + hdr.so, retx_payload_len, rlc_pdu_buf + header_len, buf_size - header_len);
    if (payload_len == 0 || payload_len != retx_payload_len) {
        printf("Error: Could not write PDU payload. sn=%u payload_len=%zu grant_len=%zu\n", hdr.sn, payload_len, grant_len);
        return 0;
    }

    size_t pdu_len = header_len + payload_len;
    printf("Info: RETX PDU. sn=%u pdu_len=%zu grant_len=%zu retx_count=%d\n", hdr.sn, pdu_len, grant_len, sdu_info->retx_count);

    entity->metrics->metrics_add_retx_pdus(1, pdu_len);

    entity->log_state(4); 

    return pdu_len;
}