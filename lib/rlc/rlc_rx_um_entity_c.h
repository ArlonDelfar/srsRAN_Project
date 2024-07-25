#ifndef RLC_RX_UM_ENTITY_C_H
#define RLC_RX_UM_ENTITY_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define UM_WINDOW_SIZE 1024

typedef struct {
    uint16_t SN;
} UMD_PDU;

typedef struct {
    uint16_t RX_Next_Highest;
    uint16_t RX_Next_Reassembly;
    uint16_t UM_Window_Size;
    bool received[UM_WINDOW_SIZE];
} rlc_rx_um_entity_t;

void handle_um_pdu_outside_reassembly_window(rlc_rx_um_entity_t *entity, UMD_PDU *pdu);

#ifdef __cplusplus
}
#endif

#endif // RLC_RX_UM_ENTITY_C_H
