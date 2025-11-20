#pragma once

#include "canzero/canzero.h"
static inline void can_defaults() {
  canzero_set_telemetry_state(telemetry_state::telemetry_state_INIT);
  canzero_set_dropped_rx_canframes(0);
  canzero_set_dropped_tx_canframes(0);
  canzero_set_dropped_rx_packets(0);
  canzero_set_dropped_tx_packets(0);

  canzero_set_accepts_new_connections(bool_t_FALSE);
  canzero_set_active_connections(0);
  canzero_set_telemetry_rx_memory_throughput(0);
  canzero_set_telemetry_tx_memory_throughput(0);
}
