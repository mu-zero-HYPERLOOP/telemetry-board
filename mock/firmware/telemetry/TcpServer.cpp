#include "firmware/telemetry/TcpServer.hpp"
#include "firmware/telemetry/SocketAddr.hpp"

#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <optional>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
namespace telemetry_board {

struct LinuxTcpConnection {
  int connectionFd;
};

struct LinuxTcpServer {
  std::uint16_t welcomePort;
  int welcomeFd;
};

TcpServer::TcpServer(std::size_t maxPacketSize, std::uint16_t port)
    : m_internals(malloc(sizeof(LinuxTcpServer))) {
  auto internals = reinterpret_cast<LinuxTcpServer *>(m_internals);
  internals->welcomePort = port;
  internals->welcomeFd = -1;
}

TcpServer::~TcpServer() { close(); }

void TcpServer::start() {
  auto internals = reinterpret_cast<LinuxTcpServer *>(m_internals);

  if (internals->welcomeFd != -1) {
    // Already started
    return;
  }

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    throw std::runtime_error("Failed to create TCP socket");
  }

  int opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(internals->welcomePort); // 0 means "any port"

  if (bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
    ::close(fd);
    throw std::runtime_error("Failed to bind TCP socket");
  }

  if (::listen(fd, 1) < 0) {
    ::close(fd);
    throw std::runtime_error("Failed to listen on TCP socket");
  }

  int flags = fcntl(fd, F_GETFL, 0);
  if (flags >= 0) {
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  }

  // If port was 0, query which port was assigned
  if (internals->welcomePort == 0) {
    sockaddr_in realAddr;
    socklen_t addrLen = sizeof(realAddr);
    if (getsockname(fd, reinterpret_cast<sockaddr *>(&realAddr), &addrLen) ==
        0) {
      internals->welcomePort = ntohs(realAddr.sin_port);
    }
  }
  assert(fd != -1);
  internals->welcomeFd = fd;
}

std::uint16_t TcpServer::welcomePort() {
  auto internals = reinterpret_cast<LinuxTcpServer *>(m_internals);
  return internals->welcomePort;
}

std::optional<TcpConnection> TcpServer::accept() {
  auto internals = reinterpret_cast<LinuxTcpServer *>(m_internals);

  if (internals->welcomeFd == -1) {
    throw std::runtime_error(
        "Welcome socket was not created. Failed to accept connection");
  }

  sockaddr_in clientAddr;
  socklen_t addrLen = sizeof(clientAddr);
  int clientFd = ::accept(internals->welcomeFd,
                          reinterpret_cast<sockaddr *>(&clientAddr), &addrLen);
  if (clientFd < 0) {
    // No client to accept (non-blocking), ignore
    return std::nullopt;
  }

  // Make client socket non-blocking
  int flags = fcntl(clientFd, F_GETFL, 0);
  if (flags >= 0) {
    int x = fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);
    assert(x >= 0);
  }
  LinuxTcpConnection *connectionInternals =
      reinterpret_cast<LinuxTcpConnection *>(
          malloc(sizeof(LinuxTcpConnection)));
  assert(connectionInternals != nullptr);
  connectionInternals->connectionFd = clientFd;
  auto x = TcpConnection(reinterpret_cast<void *>(connectionInternals));
  return std::move(x);
}

void TcpServer::close() {
  if (m_internals != nullptr) {
    auto internals = reinterpret_cast<LinuxTcpServer *>(m_internals);

    if (internals->welcomeFd != -1) {
      ::close(internals->welcomeFd);
    }

    free(m_internals);
  }
}

// TcpConnection::TcpConnection(void *internals) : m_internals(internals) {}

TcpConnection::~TcpConnection() {
  if (m_internals != nullptr) {
    close();
    free(m_internals);
  }
}

void TcpConnection::close() {
  if (m_internals != nullptr) {
    auto internals = reinterpret_cast<LinuxTcpConnection *>(m_internals);
    if (internals->connectionFd != -1) {
      ::close(internals->connectionFd);
      internals->connectionFd = -1;
    }
  }
}

TcpSendResult TcpConnection::send_exact(const void *data, std::size_t size) {
  auto internals = reinterpret_cast<LinuxTcpConnection *>(m_internals);
  assert(internals->connectionFd != -1);

  ssize_t sent = ::send(internals->connectionFd, data, size, 0);
  if (sent < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // Would block, no fatal error
      return TcpSendResult::WOULD_BLOCK;
    }
    // Fatal error, close connection
    close();
    return TcpSendResult::CLOSED;
  }

  bool succ = static_cast<std::size_t>(sent) == size;
  if (succ) {
    return TcpSendResult::SUCC;
  } else {
    // tcp connection is in a weird state afterwards we just close here
    // in the future we could implement better ways to recover
    close();
    return TcpSendResult::CLOSED;
  }
}

TcpRecvResult TcpConnection::recv_exact(void *data, std::size_t size) {
  auto internals = reinterpret_cast<LinuxTcpConnection *>(m_internals);
  assert(internals->connectionFd != -1);

  ssize_t received = ::recv(internals->connectionFd, data, size, 0);
  if (received > 0) {
    // Successfully received some data
    return TcpRecvResult::SUCC;
  } else if (received == 0) {
    // Client disconnected
    close();
    return TcpRecvResult::CLOSED;
  } else {
    // received < 0
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // No data available yet
      return TcpRecvResult::EMPTY;
    } else {
      // Other error -> assume connection broken
      close();
      return TcpRecvResult::CLOSED;
    }
  }
}

SocketAddr TcpConnection::remoteAddr() const {
  assert(m_internals != nullptr);
  auto internals = reinterpret_cast<LinuxTcpConnection *>(m_internals);
  int fd = internals->connectionFd;
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);

  if (getpeername(fd, (struct sockaddr *)&addr, &len) == -1) {
    perror("getpeername");
    // handle error
  }

  uint32_t ip = ntohl(addr.sin_addr.s_addr); // host-order IPv4 address
  uint16_t port = ntohs(addr.sin_port);

  return SocketAddr{
      .ip = ip, // <- unsigned int.
      .port = port,
  };
}

} // namespace telemetry_board
