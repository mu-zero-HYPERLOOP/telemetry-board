#include "./UdpServer.hpp"
#include "core_pins.h"
#include "print.h"
#include "qnethernet/QNEthernetUDP.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <memory>

namespace telemetry_board {

struct TeensyUdpServer {
  qindesign::network::EthernetUDP udpSocket;
  void *recvBuffer;
  std::size_t recvBufferSize;
};

UdpServer::UdpServer(std::size_t maxDatagramSize)
    : m_internals(malloc(sizeof(TeensyUdpServer))) {
  auto internals = static_cast<TeensyUdpServer *>(m_internals);
  new (internals) TeensyUdpServer();
  internals->recvBuffer = malloc(maxDatagramSize);
  internals->recvBufferSize = maxDatagramSize;
}

UdpServer::~UdpServer() {
  auto internals = static_cast<TeensyUdpServer *>(m_internals);
  free(internals->recvBuffer);
  std::destroy_at(internals);
  free(internals);
};

void UdpServer::connect(std::uint16_t port) {
  auto internals = static_cast<TeensyUdpServer *>(m_internals);
  if (!internals->udpSocket.begin(port)) {
    // Failed to begin udp socket.
  }
}

bool UdpServer::send(SocketAddr addr, const void *data, std::size_t size) {
  auto internals = static_cast<TeensyUdpServer *>(m_internals);
  return internals->udpSocket.send(IPAddress{addr.ip}, addr.port, data, size);
}

bool UdpServer::recv(void *data, std::size_t size, SocketAddr *addr) {
  auto internals = static_cast<TeensyUdpServer *>(m_internals);

  int datagramSize = internals->udpSocket.parsePacket();
  if (datagramSize < 0) {
    return false;
  }

  assert(datagramSize <= size);
  const uint8_t *datagramData = internals->udpSocket.data();
  std::memcpy(data, datagramData, datagramSize);

  addr->ip = internals->udpSocket.remoteIP();
  addr->port = internals->udpSocket.remotePort();
  return true;
}

void UdpServer::close() {
  if (m_internals != nullptr) {
    auto internals = static_cast<TeensyUdpServer *>(m_internals);
    internals->udpSocket.stop();
  }
}

} // namespace telemetry_board
