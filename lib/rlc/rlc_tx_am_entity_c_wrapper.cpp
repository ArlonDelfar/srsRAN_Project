#include "rlc_tx_am_entity_c_wrapper.h"
#include "rlc_tx_am_entity.h"

extern "C" {
    void rlc_tx_am_entity_c_wrapper_init(rlc_tx_am_entity_c_wrapper_t* wrapper, uint32_t max_retx_thresh, rlc_tx_am_sdu_info_c_t* tx_window, void (*on_max_retx)(void)) {
        rlc_tx_am_entity_c_init(&wrapper->entity, max_retx_thresh, tx_window, on_max_retx);
    }

    void rlc_tx_am_entity_c_wrapper_increment_retx_count(rlc_tx_am_entity_c_wrapper_t* wrapper, uint32_t sn) {
        rlc_tx_am_entity_c_increment_retx_count(&wrapper->entity, sn);
    }
}
