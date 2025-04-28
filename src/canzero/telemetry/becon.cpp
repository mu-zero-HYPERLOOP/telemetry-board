#include "./becon.h"
#include "canzero/telemetry/datagrams.h"
#include "canzero/telemetry/server_info.hpp"
#include "firmware/telemetry/SocketAddr.hpp"
#include "firmware/telemetry/UdpServer.hpp"

namespace canzero::telemetry::becon {

using namespace telemetry_board;

UdpServer udpServer(sizeof(Datagram));
static constexpr std::uint16_t UDP_BROADCAST_PORT = 9002;
static ServerInfo *serverInfo;

void begin(ServerInfo *info) {
  serverInfo = info;
  udpServer.connect(UDP_BROADCAST_PORT);
}

void update() {
  Datagram datagram;
  SocketAddr addr;
  if (udpServer.recv(&datagram, sizeof(datagram), &addr)) {
    if (datagram.tag == DatagramTag::Hello) {
      if (datagram.serviceName() == serverInfo->serviceName) {
        datagram = Datagram::createNetworkDescription(serverInfo);
        udpServer.send(addr, &datagram, sizeof(datagram));
      }
    }
  }
}

} // namespace canzero::telemetry::becon
