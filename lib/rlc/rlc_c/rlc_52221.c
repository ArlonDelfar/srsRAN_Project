#include "rlc_52221.h"

int sn_in_reassembly_window_c(const int sn, struct rlc_rx_um_entity* entity){
  
    if((entity->st.rx_next_highest - entity->um_window_size)<=sn && (sn < entity->st.rx_next_highest))
        return 1;

    return 0;
}
