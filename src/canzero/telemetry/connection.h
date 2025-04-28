#pragma once

#include "canzero/canzero.h"
#include "canzero/telemetry/packets.h"
#include "firmware/telemetry/TcpServer.hpp"

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
        m_state(ConnectionState::WaitingForOptionRequest) {}

  bool recv(canzero_frame *frame);

  bool send(std::uint8_t bus, canzero_frame *frame, std::uint64_t timestamp);

  void close();

  bool closed() const { return m_state == ConnectionState::Closed; }

private:
  telemetry_board::TcpConnection m_tcp;
  ConnectionState m_state;

  HandshakePacket m_optionResponse;
};

} // namespace canzero::telemetry
