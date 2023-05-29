/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "mac_ctrl/mac_scheduler_configurator.h"
#include "mac_dl/mac_scheduler_cell_info_handler.h"
#include "mac_ul/mac_scheduler_ul_buffer_state_updater.h"
#include "rach_handler/mac_scheduler_rach_handler.h"

namespace srsran {

class mac_scheduler_adapter : public mac_scheduler_configurator,
                              public mac_scheduler_ul_buffer_state_updater,
                              public mac_scheduler_cell_info_handler,
                              public mac_scheduler_rach_handler,
                              public mac_paging_information_handler
{
public:
  virtual ~mac_scheduler_adapter() = default;
};

} // namespace srsran