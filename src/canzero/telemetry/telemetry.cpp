#include "./telemetry.h"
#include "canzero/canzero.h"
#include "canzero/telemetry/becon.h"
#include "canzero/telemetry/datagrams.h"
#include "canzero/telemetry/server.h"
#include "canzero/telemetry/server_info.hpp"
#include "firmware/telemetry/TcpServer.hpp"
#include "firmware/telemetry/UdpServer.hpp"
#include "util/build_time.hpp"

namespace canzero::telemetry {

using namespace telemetry_board;

static ServerInfo serverInfo;

void begin() {
  serverInfo.servicePort = 0;
  serverInfo.timebase = Timestamp::now();
  serverInfo.networkHash = 0;
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

bool can0_recv(canzero_frame *frame) { return server::can0_recv(frame); }
bool can1_recv(canzero_frame *frame) { return server::can1_recv(frame); }

void can0_send(canzero_frame *frame) {
  // NOTE: frames may be dropped!
  (void)server::can0_send(frame);
}
void can1_send(canzero_frame *frame) {
  // NOTE: frames may be dropped!
  (void)server::can1_send(frame);
}
} // namespace canzero::telemetry
