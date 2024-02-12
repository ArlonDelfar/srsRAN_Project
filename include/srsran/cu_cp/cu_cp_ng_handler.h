/*
 *
 * Copyright 2021-2024 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "srsran/ngap/ngap.h"

namespace srsran {
namespace srs_cu_cp {

/// \brief Handler of the NG interface of the CU-CP.
///
/// This interface is used to forward NGAP messages or TNL connection updates to the CU-CP.
class cu_cp_ng_handler
{
public:
  virtual ~cu_cp_ng_handler() = default;

  /// \brief Get the NG message handler interface.
  /// \return The NG message handler interface.
  virtual ngap_message_handler& get_ngap_message_handler() = 0;

  /// \brief Get the NG event handler interface.
  /// \return The NG event handler interface.
  virtual ngap_event_handler& get_ngap_event_handler() = 0;

  /// \brief Get the state of the AMF connection.
  /// \return True if AMF is connected, false otherwise.
  virtual bool amf_is_connected() = 0;
};

} // namespace srs_cu_cp
} // namespace srsran