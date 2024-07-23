#ifndef RLC_TX_AM_ENTITY_C_WRAPPER_H
#define RLC_TX_AM_ENTITY_C_WRAPPER_H

#include "rlc_tx_am_entity_c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    rlc_tx_am_entity_c_t entity;
} rlc_tx_am_entity_c_wrapper_t;

void rlc_tx_am_entity_c_wrapper_init(rlc_tx_am_entity_c_wrapper_t* wrapper, uint32_t max_retx_thresh, rlc_tx_am_sdu_info_c_t* tx_window, void (*on_max_retx)(void));
void rlc_tx_am_entity_c_wrapper_increment_retx_count(rlc_tx_am_entity_c_wrapper_t* wrapper, uint32_t sn);

#ifdef __cplusplus
}
#endif

#endif // RLC_TX_AM_ENTITY_C_WRAPPER_H
