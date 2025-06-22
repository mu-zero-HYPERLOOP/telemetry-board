#pragma once

#include "firmware/telemetry/SocketAddr.hpp"
#include "print.h"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>

namespace telemetry_board {

enum class TcpRecvResult {
  CLOSED,
  EMPTY,
  SUCC,
};

enum class TcpSendResult {
  CLOSED,
  WOULD_BLOCK,
  SUCC,
};

class TcpConnection {
public:
  friend class TcpServer;
  TcpConnection() : m_internals(nullptr) {}
  ~TcpConnection();

  TcpConnection(const TcpConnection &) = delete;
  TcpConnection &operator=(const TcpConnection &) = delete;

  TcpConnection(TcpConnection &&o)
      : m_internals(std::exchange(o.m_internals, nullptr)) {}

  TcpConnection &operator=(TcpConnection &&o) {
    if (this == &o) {
      return *this;
    }
    close();
    m_internals = std::exchange(o.m_internals, nullptr);
    return *this;
  }

  TcpSendResult send_exact(const void *data, std::size_t size);

  TcpRecvResult recv_exact(void *data, std::size_t size);

  SocketAddr remoteAddr() const;

  void close();

private:
  TcpConnection(void *internals) : m_internals(internals) {}
  void *m_internals;
};

class TcpServer {
public:
  explicit TcpServer(std::size_t maxPacketSize, std::uint16_t port = 0);
  TcpServer() : m_internals(nullptr) {}
  TcpServer(const TcpServer &) = delete;
  TcpServer &operator=(const TcpServer &) = delete;

  TcpServer(TcpServer &&o) : m_internals(o.m_internals) {
    o.m_internals = nullptr;
  }
  TcpServer &operator=(TcpServer &&o) {
    if (this == &o) {
      return *this;
    }
    close();
    m_internals = std::exchange(o.m_internals, nullptr);
    return *this;
  }

  ~TcpServer();

  void start(); // Create tcp welcome socket


  void close();

  std::uint16_t welcomePort();

  // Always called in a loop.
  std::optional<TcpConnection> accept();

private:
  void *m_internals;
};

} // namespace telemetry_board
