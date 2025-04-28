#include "./server.h"
#include "canzero/canzero.h"
#include "canzero/telemetry/connection.h"
#include "canzero/telemetry/packets.h"
#include "canzero/telemetry/server_info.hpp"
#include "firmware/telemetry/TcpServer.hpp"
#include "util/cyclic_buffer.h"
#include "util/timestamped.h"
#include <print>

namespace canzero::telemetry::server {

static const ServerInfo *serverInfo;
static telemetry_board::TcpServer tcpServer;

std::optional<Connection> connection;

constexpr std::size_t RX_BUFFER_SIZE = 32;

static CyclicBuffer<canzero_frame, RX_BUFFER_SIZE> can0_rxQueue;
static CyclicBuffer<canzero_frame, RX_BUFFER_SIZE> can1_rxQueue;

constexpr std::size_t TX_BUFFER_SIZE = 32;
static CyclicBuffer<Timestamped<canzero_frame>, TX_BUFFER_SIZE> can0_txQueue;
static CyclicBuffer<Timestamped<canzero_frame>, TX_BUFFER_SIZE> can1_txQueue;

void begin(ServerInfo *info) {
  serverInfo = info;
  tcpServer = std::move(telemetry_board::TcpServer(
      std::max(sizeof(HandshakePacket), sizeof(Packet)),
      serverInfo->servicePort));
  tcpServer.start();
  info->servicePort = tcpServer.welcomePort();

  std::println("Started server at port={}", info->servicePort);
}

bool can0_recv(canzero_frame *frame) {
  const auto opt = can0_rxQueue.dequeue();
  if (opt.has_value()) {
    *frame = *opt;
    return true;
  } else {
    return false;
  }
}

bool can1_recv(canzero_frame *frame) {
  const auto opt = can1_rxQueue.dequeue();
  if (opt.has_value()) {
    *frame = *opt;
    return true;
  } else {
    return false;
  }
}

bool can0_send(canzero_frame *frame) {
  return can0_txQueue.enqueue(Timestamped<canzero_frame>::now(*frame));
}

bool can1_send(canzero_frame *frame) {
  return can1_txQueue.enqueue(Timestamped<canzero_frame>::now(*frame));
}

void update() {
  if (!connection.has_value()) {
    auto newConnection = tcpServer.accept();
    if (newConnection.has_value()) {
      std::println("New connection!");
      connection.emplace(std::move(*newConnection));
    }
  }

  if (connection.has_value()) {
    // Receiving frames
    canzero_frame frame;
    std::uint8_t bus;
    bool rxQueueSpace = can0_rxQueue.hasSpace() && can1_rxQueue.hasSpace();
    while (rxQueueSpace && connection->recv(&bus, &frame)) {
      if (bus == 0) {
        can0_rxQueue.enqueue(frame);
        rxQueueSpace &= can0_rxQueue.hasSpace();
      } else if (bus == 1) {
        can1_rxQueue.enqueue(frame);
        rxQueueSpace &= can0_rxQueue.hasSpace();
      } else {
        connection->close();
      }
    }

    // Sending frames
    std::optional<Timestamped<canzero_frame>> tframe;
    while ((tframe = can0_txQueue.peek()).has_value()) {
      auto time = static_cast<uint64_t>(
          static_cast<std::uint32_t>(tframe->timestamp()));
      bool ok = connection->send(0, &tframe->value(), time);
      if (ok) { can0_txQueue.dequeue(); }
      else {
        break;
      }
    }

    while ((tframe = can1_txQueue.peek()).has_value()) {
      auto time = static_cast<uint64_t>(
          static_cast<std::uint32_t>(tframe->timestamp()));
      bool ok = connection->send(1, &tframe->value(), time);
      if (ok) { can1_txQueue.dequeue(); }
      else {
        break;
      }
    }

    if (connection->closed()) {
      connection.reset();
    }
  }
}

} // namespace canzero::telemetry::server
