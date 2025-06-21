#include "./TcpServer.hpp"
#include "firmware/telemetry/SocketAddr.hpp"
#include "print.h"
#include "qnethernet/QNEthernetClient.h"
#include "qnethernet/QNEthernetServer.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <memory>

namespace telemetry_board {

struct TeensyTcpConnection {
  qindesign::network::EthernetClient client;

  TeensyTcpConnection(qindesign::network::EthernetClient &&client)
      : client(std::move(client)) {
    client.setNoDelay(true);
  }
};

TcpConnection::~TcpConnection() {
  if (m_internals != nullptr) {
    auto internals = static_cast<TeensyTcpConnection *>(m_internals);
    internals->client.abort(); // RST connection.
    internals->client.flush();
    std::destroy_at(internals);
    free(internals);
    m_internals = nullptr;
  }
}

TcpSendResult TcpConnection::send_exact(const void *data, std::size_t size) {
  auto internals = static_cast<TeensyTcpConnection *>(m_internals);
  if (!internals->client.connected()) {
    debugPrintf("Failed to send: Connection closed\n");
    close();
    return TcpSendResult::CLOSED;
  }
  auto raw = reinterpret_cast<const std::byte *>(data);
  int avail = internals->client.availableForWrite();
  if (avail >= size) {
    size_t written = 0;
    do {
      int w = internals->client.write(
          reinterpret_cast<const uint8_t *>(raw + written), size - written);
      if (w <= 0) {
        close();
        return TcpSendResult::CLOSED;
      }
      written += w;
    } while (written < size);
    return TcpSendResult::SUCC;
  } else {
    return TcpSendResult::WOULD_BLOCK;
  }
}

TcpRecvResult TcpConnection::recv_exact(void *data, std::size_t size) {
  auto internals = static_cast<TeensyTcpConnection *>(m_internals);
  if (!internals->client.connected()) {
    debugPrintf("Failed to recv: Connection closed\n");
    close();
    return TcpRecvResult::CLOSED;
  }
  auto raw = reinterpret_cast<std::byte *>(data);
  int avail = internals->client.available();

  if (avail >= size) {
    int read = 0;
    do {
      int r = internals->client.read(reinterpret_cast<uint8_t *>(raw + read),
                                     size - read);
      if (r <= 0) {
        close();
        return TcpRecvResult::CLOSED;
      }
      read += r;
    } while (read < size);
    return TcpRecvResult::SUCC;
  } else {
    return TcpRecvResult::EMPTY;
  }
}

SocketAddr TcpConnection::remoteAddr() const {
  assert(m_internals != nullptr);
  auto internals = static_cast<TeensyTcpConnection *>(m_internals);
  const auto remoteIp = internals->client.remoteIP();
  const auto remotePort = internals->client.remotePort();
  return SocketAddr{
      .ip = static_cast<unsigned int>(remoteIp),
      .port = remotePort,
  };
}

void TcpConnection::close() {
  if (m_internals != nullptr) {
    auto internals = static_cast<TeensyTcpConnection *>(m_internals);
    internals->client.close();
  }
}

/*
 * ================= TCP-SERVER ==================
 */

struct TeensyTcpServer {
  qindesign::network::EthernetServer welcomeServer;

  TeensyTcpServer(uint16_t port) : welcomeServer(port) {}
};

TcpServer::TcpServer(std::size_t maxPacketSize, std::uint16_t port)
    : m_internals(malloc(sizeof(TeensyTcpServer))) {
  auto internals = static_cast<TeensyTcpServer *>(m_internals);
  new (internals) TeensyTcpServer(port);
}

TcpServer::~TcpServer() {
  if (m_internals != nullptr) {
    auto internals = static_cast<TeensyTcpServer *>(m_internals);
    std::destroy_at(internals);
    free(internals);
  }
}

void TcpServer::start() {
  auto internals = static_cast<TeensyTcpServer *>(m_internals);
  assert(internals != nullptr);
  internals->welcomeServer.begin();
}

void TcpServer::close() {
  if (m_internals != nullptr) {
    auto internals = static_cast<TeensyTcpServer *>(m_internals);
    internals->welcomeServer.end();
  }
}

std::uint16_t TcpServer::welcomePort() {
  auto internals = static_cast<TeensyTcpServer *>(m_internals);
  return internals->welcomeServer.port();
}

// Always called in a loop.
std::optional<TcpConnection> TcpServer::accept() {
  auto internals = static_cast<TeensyTcpServer *>(m_internals);
  qindesign::network::EthernetClient client = internals->welcomeServer.accept();
  if (!client) {
    return std::nullopt;
  }
  auto connection =
      static_cast<TeensyTcpConnection *>(malloc(sizeof(TeensyTcpConnection)));
  new (connection) TeensyTcpConnection(std::move(client));
  return TcpConnection(connection);
}

} // namespace telemetry_board
