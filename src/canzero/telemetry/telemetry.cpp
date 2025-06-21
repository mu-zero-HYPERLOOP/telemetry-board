#include "./telemetry.h"
#include "canzero/canzero.h"
#include "canzero/telemetry/becon.h"
#include "canzero/telemetry/server.h"
#include "canzero/telemetry/server_info.hpp"
#include "util/build_time.hpp"

namespace canzero::telemetry {

static ServerInfo serverInfo;

void begin() {
  serverInfo.servicePort = 0;
  serverInfo.timebase = Timestamp::now();
  serverInfo.networkHash = canzero_get_config_hash();
  serverInfo.serverName = "Athena-SV";
  serverInfo.serviceName = "CANzero";
  serverInfo.buildTime = buildTime();
  server::begin(&serverInfo);
  becon::begin(&serverInfo);
}

void update() {

  server::update(); // tcp welcome socket and connections
  becon::update();  // udp becon!
}

bool can_recv(uint8_t bus, canzero_frame *frame) {
  return server::can_recv(bus, frame);
}

bool can_send(uint8_t bus, canzero_frame *frame) {
  return server::can_send(bus, frame);
}

} // namespace canzero::telemetry
