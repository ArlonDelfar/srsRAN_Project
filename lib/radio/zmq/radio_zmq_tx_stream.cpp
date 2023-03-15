/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "radio_zmq_tx_stream.h"

using namespace srsran;

radio_zmq_tx_stream::radio_zmq_tx_stream(void*                       zmq_context,
                                         const stream_description&   config,
                                         task_executor&              async_executor_,
                                         radio_notification_handler& notification_handler)
{
  // For each channel...
  for (unsigned channel_id = 0; channel_id != config.address.size(); ++channel_id) {
    // Prepare configuration.
    radio_zmq_tx_channel::channel_description channel_description;
    channel_description.socket_type       = config.socket_type;
    channel_description.address           = config.address[channel_id];
    channel_description.stream_id         = config.stream_id;
    channel_description.channel_id        = channel_id;
    channel_description.channel_id_str    = config.stream_id_str + ":" + std::to_string(channel_id);
    channel_description.log_level         = config.log_level;
    channel_description.trx_timeout_ms    = config.trx_timeout_ms;
    channel_description.linger_timeout_ms = config.linger_timeout_ms;
    channel_description.buffer_size       = config.buffer_size;

    // Create channel.
    channels.emplace_back(std::make_unique<radio_zmq_tx_channel>(
        zmq_context, channel_description, notification_handler, async_executor_));

    // Check if the channel construction was successful.
    if (!channels.back()->is_successful()) {
      return;
    }
  }

  successful = true;
}

void radio_zmq_tx_stream::stop()
{
  for (auto& channel : channels) {
    channel->stop();
  }
}

void radio_zmq_tx_stream::wait_stop()
{
  for (auto& channel : channels) {
    channel->wait_stop();
  }
}

bool radio_zmq_tx_stream::align(uint64_t timestamp, std::chrono::milliseconds timeout)
{
  // Returns true if at least one channel is in the past.
  bool timestamp_passed = false;
  for (auto& channel : channels) {
    timestamp_passed = timestamp_passed || channel->align(timestamp, timeout);
  }
  return timestamp_passed;
}

void radio_zmq_tx_stream::transmit(baseband_gateway_buffer& data)
{
  report_fatal_error_if_not(data.get_nof_channels() == channels.size(),
                            "Invalid number of channels ({}) expected {}.",
                            data.get_nof_channels(),
                            channels.size());

  for (unsigned channel_id = 0; channel_id != channels.size(); ++channel_id) {
    channels[channel_id]->transmit(data.get_channel_buffer(channel_id));
  }
}
