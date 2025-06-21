#include "./server.h"
#include "QNEthernet.h"
#include "can_bus_count.hpp"
#include "canzero/canzero.h"
#include "canzero/telemetry/connection.h"
#include "canzero/telemetry/ip_host.h"
#include "canzero/telemetry/packets.h"
#include "canzero/telemetry/server_config.h"
#include "canzero/telemetry/server_info.hpp"
#include "core_pins.h"
#include "firmware/telemetry/SocketAddr.hpp"
#include "firmware/telemetry/TcpServer.hpp"
#include "print.h"
#include "util/cyclic_buffer.h"
#include "util/static_bag.h"
#include "util/timing.h"
#include <algorithm>
#include <cstring>

namespace canzero::telemetry::server {

static const ServerInfo *serverInfo;
static telemetry_board::TcpServer tcpServer;

StaticBag<Connection, MAX_AMOUNT_OF_CONNECTIONS> connections;

static ConnectionIdHost connectionIdHost;

// TODO useless indirection (i think, unless we want to broadcast to other
// connections as well).
static constexpr std::size_t RX_QUEUE_SIZE = 4096;
std::array<CyclicBuffer<canzero_frame, RX_QUEUE_SIZE>, CAN_BUS_COUNT>
    can_rxQueues;

void begin(ServerInfo *info) {
  serverInfo = info;
  tcpServer = telemetry_board::TcpServer(
      std::max(sizeof(HandshakePacket), sizeof(Packet)),
      serverInfo->servicePort);
  debugPrintFlush();
  tcpServer.start();
  info->servicePort = tcpServer.welcomePort();

  debugPrintf("Started TCP-Server:\n");
  {
    char cServiceName[128];
    std::memset(cServiceName, 0, 128);
    std::memcpy(cServiceName, info->serviceName.data(),
                info->serviceName.size() * sizeof(char));
    debugPrintf(" - service-name : %s\n", cServiceName);
  }
  debugPrintf(" - port : %d\n", info->servicePort);
  debugPrintf(" - config-hash : %d\n", info->networkHash);
  {
    char cBuildTime[128];
    std::memset(cBuildTime, 0, 128);
    std::memcpy(cBuildTime, info->buildTime.data(),
                info->buildTime.size() * sizeof(char));
    debugPrintf(" - build-time : %s\n", cBuildTime);
    debugPrintf(" - supports : [idreq]\n");
    debugPrintf(" - not_supported : [sync]\n");
  }
}

bool can_recv(uint8_t bus, canzero_frame *frame) {
  assert(bus < CAN_BUS_COUNT);
  const auto opt = can_rxQueues[bus].dequeue();
  if (opt.has_value()) {
    *frame = opt.value();
    return true;
  }
  return false;
}

bool can_send(uint8_t bus, canzero_frame *frame) {
  Timing timing;
  timing.start();
  assert(bus < CAN_BUS_COUNT);
  const auto time = Timestamp::now() - serverInfo->timebase;
  const Packet packet = Packet::createNetworkFrame(bus, frame, time.as_us());
  bool succ = true;
  for (std::uint32_t i = 0; i < connections.size(); ++i) {
    auto &connection = connections[i];
    if (!connection.send(&packet)) {
      succ = false;
    }
  }
  // debugPrintf("server::can_send took %dus\n", timing.time().as_us());
  return succ;
}

static bool allRxQueuesHaveSpace() {
  return std::all_of(can_rxQueues.begin(), can_rxQueues.end(),
                     [](auto &q) { return q.hasSpace(); });
}

void update() {
  // debugPrintf("Update server\n");

  if (connections.size() < MAX_AMOUNT_OF_CONNECTIONS) {
    auto newConnection = tcpServer.accept();

    if (newConnection.has_value()) {
      debugPrintf("[TCP-Server] New connection from ");
      telemetry_board::printSocketAddress(newConnection->remoteAddr());
      debugPrintf("\n");
      connections.add(Connection(std::move(*newConnection), &connectionIdHost));
    }
  }

  for (std::uint32_t i = 0; i < connections.size(); ++i) {
    auto &connection = connections[i];

    {
      Timing timing;
      timing.start();
      connection.update();
      if (timing.time() > 1_ms) {
        debugPrintf("server::update update connection took %dus\n",
                    timing.time().as_us());
      }
    }

    qindesign::network::Ethernet.loop();

    if (connection.closed()) {
      debugPrintf("[TCP-Server] Closed connection from ");
      telemetry_board::printSocketAddress(connection.remoteAddr());
      debugPrintf("\n");
      connections.remove(i);
      i--;
      continue;
    }

    Packet packet;
    while (allRxQueuesHaveSpace() && connection.recv(&packet)) {
      // forward to other clients
      for (std::uint32_t j = 0; j < connections.size(); ++j) {
        if (j == i) {
          continue;
        }
        auto &peer = connections[j];
        (void)peer.send(&packet);
      }

      // forward to application layer
      canzero_frame frame;
      frame.id = packet.canId;
      frame.dlc = packet.dlc;
      std::memcpy(frame.data, &packet.data, sizeof(std::uint64_t));
      if (packet.bus < CAN_BUS_COUNT) {
        can_rxQueues[packet.bus].enqueue(frame);
      } else {
        // Warning: Received invalid tcp frame.
      }
    }

    if (!allRxQueuesHaveSpace()) {
      debugPrintf("Blocked by rx queue length\n");
    }
  }
}

} // namespace canzero::telemetry::server
