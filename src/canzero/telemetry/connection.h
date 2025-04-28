#pragma once

#include "canzero/canzero.h"
#include "canzero/telemetry/packets.h"
#include "firmware/telemetry/TcpServer.hpp"
#include <print>

namespace canzero::telemetry {

enum class ConnectionState {
  Closed,
  WaitingForOptionRequest,
  ReceivedOptionRequest,
  Communication,
};

class Connection {
public:
  Connection(telemetry_board::TcpConnection tcp)
      : m_tcp(std::move(tcp)),
        m_state(ConnectionState::WaitingForOptionRequest) {
    std::println("Waiting for option request");
  }
  Connection() : m_tcp{}, m_state(ConnectionState::Closed) {}
  Connection(const Connection &) = delete;
  Connection &operator=(const Connection &) = delete;

  Connection(Connection &&o)
      : m_tcp{std::move(o.m_tcp)}, m_state(o.m_state),
        m_optionResponse(o.m_optionResponse) {
    o.m_state = ConnectionState::Closed;
  }
  Connection &operator=(Connection &&o) {
    if (this == &o) {
      return *this;
    }
    close();
    std::swap(m_tcp, o.m_tcp);
    std::swap(m_state, o.m_state);
    m_optionResponse = o.m_optionResponse;
    return *this;
  }

  bool recv(std::uint8_t *bus, canzero_frame *frame);

  bool send(std::uint8_t bus, canzero_frame *frame, std::uint64_t timestamp);

  void close();

  bool closed() const { return m_state == ConnectionState::Closed; }

private:
  telemetry_board::TcpConnection m_tcp;
  ConnectionState m_state;

  HandshakePacket m_optionResponse;
};

} // namespace canzero::telemetry
