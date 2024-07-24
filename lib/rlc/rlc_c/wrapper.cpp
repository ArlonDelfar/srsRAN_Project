#include "wrapper.hpp"

bool sn_in_reassembly_window(const uint32_t sn, struct rlc_rx_um_entity* entity){
    return sn_in_reassembly_window_c(sn, entity);
}