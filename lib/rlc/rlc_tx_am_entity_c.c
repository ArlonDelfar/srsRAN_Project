#include "rlc_tx_am_entity_c.h"
#include <stdio.h>

void rlc_tx_am_entity_c_init(rlc_tx_am_entity_c_t* entity, uint32_t max_retx_thresh, rlc_tx_am_sdu_info_c_t* tx_window, void (*on_max_retx)(void)) {
    entity->max_retx_thresh = max_retx_thresh;
    entity->tx_window = tx_window;
    entity->on_max_retx = on_max_retx;
}

void rlc_tx_am_entity_c_increment_retx_count(rlc_tx_am_entity_c_t* entity, uint32_t sn) {
    rlc_tx_am_sdu_info_c_t* pdu = &entity->tx_window[sn];
    if (pdu->retx_count == RETX_COUNT_NOT_INITIATED) {
        pdu->retx_count = 0;
    } else {
        pdu->retx_count++;
    }
    
    if (pdu->retx_count == entity->max_retx_thresh) {
        printf("Reached maximum number of RETX. sn=%u retx_count=%u\n", sn, pdu->retx_count);
        if (entity->on_max_retx) {
            entity->on_max_retx();
        }
    }
}
