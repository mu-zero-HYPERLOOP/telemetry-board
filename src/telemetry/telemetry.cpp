#include "./telemetry.h"
#include "canzero/canzero.h"
#include "firmware/LinkState.hpp"
#include "firmware/telemetry_board.hpp"
#include "print.h"
#include "telemetry/becon.h"
#include "telemetry/packets.h"
#include "telemetry/server.h"
#include "telemetry/server_info.hpp"
#include "util/build_time.hpp"
#include "util/ema.h"
#include "util/interval.h"
#include "util/timing.h"

namespace telemetry {

static ServerInfo serverInfo;

static bool running;

static uint32_t droppedTxPackets;
static uint32_t restartCount = 0; // TODO canzero OE.

IntervalTiming txPacketFrequencyTiming{0.001};
IntervalTiming rxPacketFrequencyTiming{0.001};

Interval bandwidthMeasInterval{10_Hz};

static void start() {
  restartCount += 1;
  txPacketFrequencyTiming.reset();
  rxPacketFrequencyTiming.reset();
  debugPrintf("Starting telemetry\n");
  server::begin(&serverInfo);
  becon::begin(&serverInfo);
  running = true;
}

static void stop() {
  debugPrintf("Stopping telemetry\n");
  running = false;
  server::end();
  becon::end();
}

static void onLinkStateChange(telemetry_board::LinkState linkState) {
  using namespace telemetry_board;
  // error handling.
  if (linkState & LinkStateBits::ERROR) {
    if (linkState & LinkStateBits::ERROR_BEGIN_ETHERNET) {
    }
    if (linkState & LinkStateBits::ERROR_INET_DOWN) {
    }
    if (linkState & LinkStateBits::ERROR_LINK_TIMEOUT) {
    }
    if (running) {
      stop();
    }
    return;
  }
  if ((linkState & LinkStateBits::READY) == LinkStateBits::READY) {
    if (!running) {
      start();
    }
  } else {
    if (running) {
      stop();
    }
  }
}

void begin() {
  restartCount = 0;
  serverInfo.servicePort = 0; // dynamic port.
  serverInfo.timebase = Timestamp::now();
  serverInfo.networkHash = canzero_get_config_hash();
  serverInfo.serverName = "Polaris";
  serverInfo.serviceName = "CANzero";
  serverInfo.buildTime = buildTime();

  telemetry_board::onLinkStateChange(onLinkStateChange);
}

void update() {
  if (running) {
    server::update(); // tcp welcome socket and connections
    becon::update();  // udp becon!
    canzero_set_dropped_tx_packets(droppedTxPackets);

    if (bandwidthMeasInterval.next()) {
      Frequency txPacketFreq = txPacketFrequencyTiming.frequency();

      canzero_set_telemetry_tx_memory_throughput(
          (txPacketFreq * sizeof(Packet) * 8) / 1_kHz);

      Frequency rxPacketFreq = rxPacketFrequencyTiming.frequency();

      canzero_set_telemetry_rx_memory_throughput(
          (rxPacketFreq * sizeof(Packet) * 8) / 1_kHz);
    }

    canzero_set_accepts_new_connections(server::listening() ? bool_t_TRUE
                                                            : bool_t_FALSE);
    canzero_set_active_connections(server::activeConnectionCount());

  } else {
    canzero_set_dropped_tx_packets(0);
    canzero_set_telemetry_tx_memory_throughput(0.0f);
    canzero_set_telemetry_rx_memory_throughput(0.0f);
    canzero_set_accepts_new_connections(bool_t_FALSE);
    canzero_set_active_connections(0);
  }
  if (running) {
    if (server::activeConnectionCount() > 0) {
      canzero_set_telemetry_state(
          telemetry_state::telemetry_state_CLIENT_CONNECTED);
    } else {
      canzero_set_telemetry_state(telemetry_state::telemetry_state_LISTENING);
    }
  } else {
    canzero_set_telemetry_state(telemetry_state::telemetry_state_DISCONNECTED);
  }

  telemetry_board::led(running);
}

bool can_recv(uint8_t bus, canzero_frame *frame) {
  if (running) {
    bool succ = server::can_recv(bus, frame);
    if (succ) {
      rxPacketFrequencyTiming.tick();
    }
    return succ;
  } else {
    return false;
  }
}

bool can_send(uint8_t bus, canzero_frame *frame) {
  if (running) {
    bool succ = server::can_send(bus, frame);
    if (!succ) {
      droppedTxPackets += 1;
    } else {
      txPacketFrequencyTiming.tick();
    }
    return succ;
  } else {
    return false;
  }
}

} // namespace telemetry
