#pragma once

#include "canzero/canzero.h"
#include "telemetry/ip_host.h"
#include "telemetry/packets.h"
#include "firmware/telemetry/SocketAddr.hpp"
#include "firmware/telemetry/TcpServer.hpp"
#include "util/cyclic_buffer.h"
#include "util/interval.h"
#include <utility>

namespace telemetry {

enum class ConnectionState {
  Closed,
  WaitingForOptionRequest,
  ReceivedOptionRequest,
  Communication,
};

class Connection {
private:
  struct NetworkFrame {
    std::uint8_t bus;
    canzero_frame frame;
  };

public:
  Connection(telemetry_board::TcpConnection tcp,
             server::ConnectionIdHost *idHost)
      : m_tcp(std::move(tcp)), m_remoteAddr(m_tcp.remoteAddr()),
        m_state(ConnectionState::WaitingForOptionRequest), m_idHost(idHost) {}
  Connection() : m_tcp{}, m_state(ConnectionState::Closed) {}
  Connection(const Connection &) = delete;
  Connection &operator=(const Connection &) = delete;

  Connection(Connection &&o)
      : m_tcp{std::move(o.m_tcp)}, m_remoteAddr(o.m_remoteAddr),
        m_state(o.m_state), m_idHost(std::exchange(o.m_idHost, nullptr)),
        m_optionResponse(o.m_optionResponse) {
    o.m_state = ConnectionState::Closed;
  }
  Connection &operator=(Connection &&o) {
    if (this == &o) {
      return *this;
    }
    close();
    m_tcp = std::exchange(o.m_tcp, {});
    m_remoteAddr = o.m_remoteAddr;
    m_state = std::exchange(o.m_state, ConnectionState::Closed);
    m_idHost = std::exchange(o.m_idHost, nullptr);
    m_optionResponse = o.m_optionResponse;
    return *this;
  }
  ~Connection() {
    close();
    if (m_uniqueId.has_value()) {
      m_idHost->free_id(m_uniqueId.value());
    }
  }

  bool recv(Packet *packet);

  bool send(const Packet *packet);

  void update();

  void close();

  const telemetry_board::SocketAddr &remoteAddr() const { return m_remoteAddr; }

  bool closed() const { return m_state == ConnectionState::Closed; }

private:
  telemetry_board::TcpConnection m_tcp;
  telemetry_board::SocketAddr m_remoteAddr;
  ConnectionState m_state;
  server::ConnectionIdHost *m_idHost;

  HandshakePacket m_optionResponse;

  static constexpr std::size_t RX_QUEUE_SIZE = 512;
  CyclicBuffer<Packet, RX_QUEUE_SIZE> m_rxQueue;

  static constexpr std::size_t TX_QUEUE_SIZE = 128;
  CyclicBuffer<Packet, TX_QUEUE_SIZE> m_txQueue;

  std::optional<uint8_t> m_uniqueId;

  Interval m_keepAliveInterval{250_ms};
};

} // namespace canzero::telemetry
