#ifndef RLC_TX_AM_ENTITY_C_H
#define RLC_TX_AM_ENTITY_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define RETX_COUNT_NOT_INITIATED 0xffffffff

typedef struct {
    uint32_t retx_count;
    bool pending_retransmission;
    bool retx_incremented_in_status_pdu;
} rlc_tx_am_sdu_info_c_t;

typedef struct {
    rlc_tx_am_sdu_info_c_t* tx_window;
    uint32_t max_retx_thresh;
    void (*on_max_retx)(void);
} rlc_tx_am_entity_c_t;

void rlc_tx_am_entity_c_init(rlc_tx_am_entity_c_t* entity, uint32_t max_retx_thresh, rlc_tx_am_sdu_info_c_t* tx_window, void (*on_max_retx)(void));
void rlc_tx_am_entity_c_increment_retx_count(rlc_tx_am_entity_c_t* entity, uint32_t sn);

#ifdef __cplusplus
}
#endif

#endif // RLC_TX_AM_ENTITY_C_H
