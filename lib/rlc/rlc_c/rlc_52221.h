#include <stdint.h>

#ifndef RLC_52221
#define RLC_52221

//
struct rlc_rx_um_state
{
    int rx_next_highest;
};

struct rlc_rx_um_entity
{
    struct rlc_rx_um_state st;
    const uint32_t um_window_size;
};

int sn_in_reassembly_window_c(const int sn, struct rlc_rx_um_entity* entity);
#endif

