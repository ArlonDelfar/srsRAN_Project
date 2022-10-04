/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "../du_ue/cell_group_config.h"
#include "srsgnb/asn1/rrc_nr/rrc_nr.h"

namespace srsgnb {
namespace srs_du {

asn1::rrc_nr::coreset_s make_asn1_rrc_coreset(const coreset_configuration& cfg);

asn1::rrc_nr::search_space_s make_asn1_rrc_search_space(const search_space_configuration& cfg);

void calculate_cell_group_config_diff(const cell_group_config&        src,
                                      const cell_group_config&        dest,
                                      asn1::rrc_nr::cell_group_cfg_s& out);

} // namespace srs_du
} // namespace srsgnb