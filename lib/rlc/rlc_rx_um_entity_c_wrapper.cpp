#include "rlc_rx_um_entity.h"
#include "rlc_rx_um_entity_c_wrapper.h"
#include "include\srsran\rlc\rlc_config.h"

struct rlc_rx_um_entity_t
{
    std::unique_ptr<srsran::rlc_rx_um_entity> impl;
};

extern "C" {
    rlc_rx_um_entity_t* rlc_rx_um_entity_create (
        int gnb_du_id,
        int ue_index,
        int rb_id,
        int config_sn_field_length,
        int config_t_reassembly,
        bool metrics_enabled
    ) 
    {
        rlc_rx_um_config config;
        config.sn_field_length = static_cast<srsran::rlc_um_sn_size>(config_sn_field_length);
        config.t_reassembly = config_t_reassembly;

        rlc_rx_upper_layer_datanotofier upper_dn;
        timer_factory timers;
        task_executor ue_executor;
        rlc_pcap pcap;

        rlc_rx_um_entity_t* entity = new rlc_rx_um_entity_t();
        entity->impl = std::make_unique<srsran::rlc_rx_um_entity>(
            gnb_du_id,
            ue_index,
            rb_id,
            config,
            upper_dn,
            timers,
            ue_executor,
            metrics_enabled,
            pcap
        );
    
        return entity;
    }

    void rlc_rx_um_entity_handle_pdu(rlc_rx_um_entity_t* entity, uint8_t* data, size_t length){
        srsran::byte_buffer_slice buffer(data, length);
        entity->impl->handle_pdu(std::move(buffer));
    }

    void rlc_rx_um_entity_check_reassembly_timer(rlc_rx_um_entity_t* entity)
    {
        if (entity->impl->reassembly_timer.is_running()) {
            bool stop_reassembly_timer = false;
            auto& st = entity->impl->st;
            auto& rx_window = entity->impl->rx_window;

            if (st.rx_timer_trigger <= st.rx_next_reassembly) {
                stop_reassembly_timer = true;
            }
            if (!sn_in_reassembly_window(st.rx_timer_trigger) && st.rx_timer_trigger != st.rx_next_highest) {
                stop_reassembly_timer = true;
            }
            if (st.rx_next_highest == st.rx_next_reassembly + 1) {
                if (rx_window->has_sn(st.rx_next_highest) && not(*rx_window)[st.rx_next_highest].has_gap) {
                    stop_reassembly_timer = true;
                }
            }
            if (stop_reassembly_timer) {
                entity->impl->reassembly_timer.stop();
                entity->impl->logger.log_debug("Stopped reassembly_timer at sn={}.", header.sn);
        }
    }
    }
}