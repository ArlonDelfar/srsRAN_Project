#include "rlc_rx_um_entity_c.h"

void handle_um_pdu_outside_reassembly_window(rlc_rx_um_entity_t *entity, UMD_PDU *pdu)
{
    entity->RX_Next_Highest = pdu->SN + 1;

    for (uint16_t i = 0; i < UM_WINDOW_SIZE; i++) {
        if (i < entity->RX_Next_Highest - entity->UM_Window_Size) {
            entity->received[i] = false;
        }
    }

    if (entity->RX_Next_Reassembly < entity->RX_Next_Highest - entity->UM_Window_Size) {
        for (uint16_t i = entity->RX_Next_Highest - entity->UM_Window_Size; i < entity->RX_Next_Highest; i++) {
            if (!entity->received[i]) {
                entity->RX_Next_Reassembly = i;
                break;
            }
        }
    }
}
