#ifndef RLC_TX_AM_ENTITY_C_H
#define RLC_TX_AM_ENTITY_C_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    uint16_t sn;
    size_t so;
    size_t length;
} rlc_tx_amd_retx;

typedef struct {
    size_t length;
    uint8_t* data;
} sdu_info_t;

typedef struct {
    sdu_info_t sdu;
    int retx_count;
} rlc_tx_am_sdu_info;

typedef struct {
    bool (*has_sn)(void* tx_window, uint16_t sn);
    rlc_tx_am_sdu_info* (*get_sdu_info)(void* tx_window, uint16_t sn);
} tx_window_t;

typedef struct {
    bool (*empty)();
    rlc_tx_amd_retx (*front)();
    void (*pop)();
    void (*replace_front)(rlc_tx_amd_retx);
} retx_queue_t;

typedef enum {
    rlc_dc_field_data
} rlc_dc_field;

typedef enum {
    rlc_si_field_full_sdu,
    rlc_si_field_first_segment,
    rlc_si_field_middle_segment,
    rlc_si_field_last_segment
} rlc_si_field;

typedef struct {
    rlc_dc_field dc;
    int p;
    rlc_si_field si;
    size_t sn_size;
    uint16_t sn;
    size_t so;
} rlc_am_pdu_header;

typedef struct {
    size_t (*metrics_add_retx_pdus)(int, size_t);
} metrics_t;

typedef struct {
    tx_window_t* tx_window;
    retx_queue_t* retx_queue;
    metrics_t* metrics;
    int (*get_polling_bit)(uint16_t sn, bool is_retx, int);
    size_t (*get_retx_expected_hdr_len)(rlc_tx_amd_retx);
    size_t (*rlc_am_write_data_pdu_header)(uint8_t* rlc_pdu_buf, rlc_am_pdu_header hdr);
    size_t (*copy_segments)(uint8_t* source, size_t source_len, uint8_t* destination, size_t destination_len);
    void (*log_state)(int level);
    struct {
        size_t sn_field_length;
    } cfg;
    uint16_t retx_sn;
} rlc_tx_am_entity_t;

size_t build_retx_pdu(rlc_tx_am_entity_t* entity, uint8_t* rlc_pdu_buf, size_t buf_size);

#endif // RLC_TX_AM_ENTITY_C_H