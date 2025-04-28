#include "./telemetry.h"
#include "canzero/canzero.h"
#include "canzero/telemetry/becon.h"
#include "canzero/telemetry/datagrams.h"
#include "canzero/telemetry/server_info.hpp"
#include "firmware/telemetry/TcpServer.hpp"
#include "firmware/telemetry/UdpServer.hpp"
#include "util/build_time.hpp"

namespace canzero::telemetry {

using namespace telemetry_board;

static ServerInfo serverInfo;

void begin() {
  serverInfo.timebase = Timestamp::now();
  serverInfo.networkHash = 0;
  serverInfo.servicePort = 1123;
  serverInfo.serverName = "Athena-SV";
  serverInfo.serviceName = "CANzero";
  serverInfo.buildTime = buildTime();
  becon::begin(&serverInfo);
}

void update() {
  becon::update(); // udp becon!
}

bool can0_recv(canzero_frame *frame) { return false; }
bool can1_recv(canzero_frame *frame) { return false; }

void can0_send(canzero_frame *frame) {}
void can1_send(canzero_frame *frame) {}
} // namespace canzero::telemetry

