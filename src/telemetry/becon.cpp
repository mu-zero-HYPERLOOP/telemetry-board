#include "./becon.h"
#include "telemetry/datagrams.h"
#include "telemetry/server_info.hpp"
#include "firmware/telemetry/SocketAddr.hpp"
#include "firmware/telemetry/UdpServer.hpp"

namespace telemetry::becon {

using namespace telemetry_board;

static UdpServer udpServer(sizeof(Datagram));
static constexpr std::uint16_t UDP_BROADCAST_PORT = 9002;
static ServerInfo *serverInfo;

void begin(ServerInfo *info) {
  serverInfo = info;
  udpServer.connect(UDP_BROADCAST_PORT);
  debugPrintf("Started UDP-Becon:\n");
  debugPrintf(" - port : %d\n", UDP_BROADCAST_PORT);
}

void end() {
  udpServer.close();
}

void update() {
  Datagram datagram;
  SocketAddr addr;
  // NOTE: We assume that a UdpServer implements proper UDP semantics here
  // i.e. we either receive a full datagram or nothing. Same for sending.
  if (udpServer.recv(&datagram, sizeof(datagram), &addr)) {
    if (datagram.tag == DatagramTag::Hello) {
      if (datagram.serviceName() == serverInfo->serviceName) {
        datagram = Datagram::createNetworkDescription(serverInfo);
        // may fail, but then we just have to send another hello frame
        // from the client side.
        udpServer.send(addr, &datagram, sizeof(datagram));
      }
    }
  }
}

} // namespace canzero::telemetry::becon
