#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
namespace telemetry_board {

enum class TcpRecvInfo {
  CLOSED,
  EMPTY,
  SUCC,
};

enum class TcpSendInfo {
  CLOSED,
  SKIPPED,
  SUCC,
};

class TcpConnection {
public:
  friend class TcpServer;
  TcpConnection() : m_internals(nullptr) {}
  ~TcpConnection();
  TcpConnection(const TcpConnection &) = delete;
  TcpConnection &operator=(const TcpConnection &) = delete;
  __attribute__((weak)) TcpConnection(TcpConnection &&o)
      : m_internals(o.m_internals) {
    o.m_internals = nullptr;
  }
  __attribute__((weak)) TcpConnection &operator=(TcpConnection &&o) {
    if (this == &o) {
      return *this;
    }
    this->~TcpConnection(); // hacky
    m_internals = o.m_internals;
    o.m_internals = nullptr;
    return *this;
  }

  TcpSendInfo send(void *data, std::size_t size);
  TcpRecvInfo recv(void *data, std::size_t size);

  void close();

private:
  TcpConnection(void *internals);
  void *m_internals;
};

class TcpServer {
public:
  explicit TcpServer(std::size_t maxPacketSize, std::uint16_t port = 0);

  ~TcpServer();

  void start(); // Create tcp welcome socket

  std::uint16_t welcomePort();

  // Always called in a loop.
  std::optional<TcpConnection> accept();

private:
  void *m_internals;
};

} // namespace telemetry_board
