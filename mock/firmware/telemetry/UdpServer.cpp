#include "firmware/telemetry/UdpServer.hpp"
#include <arpa/inet.h> // for htons()
#include <asm-generic/socket.h>
#include <cassert>
#include <cstdlib>
#include <cstring> // for std::memset
#include <fcntl.h> // for fcntl()
#include <netinet/in.h> // for sockaddr_in
#include <stdexcept>
#include <sys/socket.h> // for socket functions
#include <unistd.h>     // for close()

namespace telemetry_board {

struct LinuxUdpServer {
  int socket_fd;
  void *recvBuffer;
  std::size_t recvBufferSize;
};

UdpServer::UdpServer(std::size_t maxDatagramSize)
    : m_internals(malloc(sizeof(LinuxUdpServer))) {
  auto internals = reinterpret_cast<LinuxUdpServer *>(m_internals);
  internals->recvBuffer = malloc(maxDatagramSize);
  internals->recvBufferSize = maxDatagramSize;
  internals->socket_fd = -1;
}

void UdpServer::connect(std::uint16_t port) {
  auto internals = reinterpret_cast<LinuxUdpServer *>(m_internals);

  if (internals->socket_fd == -1) {
    close(internals->socket_fd);
  }

  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    throw std::runtime_error("Failed to create udp-socket");
  }
  int broadcastEnable = -1;
  if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable,
                 sizeof(broadcastEnable)) < 0) {
    close(fd);
    fd = -1;
    throw std::runtime_error("Failed to set SO_BROADCAST for udp-socket");
  }
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
    close(fd);
    fd = -1;
    throw std::runtime_error("Failed to make udp-socket non-blocking");
  }

  sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

  if (bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
    close(fd);
    fd = -1;
    throw std::runtime_error("Failed to bind udp-socket");
  }

  internals->socket_fd = fd;
}

UdpServer::~UdpServer() {
  auto internals = reinterpret_cast<LinuxUdpServer *>(m_internals);
  close(internals->socket_fd);
  internals->socket_fd = -1;
  free(internals->recvBuffer);
  free(m_internals);
}

bool UdpServer::send(SocketAddr addr, const void *data, std::size_t size) {
  auto internals = reinterpret_cast<LinuxUdpServer *>(m_internals);
  int fd = internals->socket_fd;
  if (fd < 0) {
    return false;
  }

  sockaddr_in destAddr;
  std::memset(&destAddr, 0, sizeof(destAddr));
  destAddr.sin_family = AF_INET;
  destAddr.sin_port = htons(addr.port);
  destAddr.sin_addr.s_addr = htonl(addr.ip);

  ssize_t sent =
      ::sendto(fd, data, size, 0, reinterpret_cast<sockaddr *>(&destAddr),
               sizeof(destAddr));
  return sent == static_cast<ssize_t>(size);
}

bool UdpServer::recv(void *data, std::size_t size, SocketAddr *addr) {
  auto internals = reinterpret_cast<LinuxUdpServer *>(m_internals);
  assert(internals->recvBufferSize <= size);
  int fd = internals->socket_fd;
  if (fd < 0) {
    return false;
  }

  sockaddr_in srcAddr;
  socklen_t srcAddrLen = sizeof(srcAddr);
  sockaddr *srcAddrPtr =
      addr ? reinterpret_cast<sockaddr *>(&srcAddr) : nullptr;
  socklen_t *srcAddrLenPtr = addr ? &srcAddrLen : nullptr;

  ssize_t received =
      ::recvfrom(fd, data, size, 0,
                 srcAddrPtr, srcAddrLenPtr);
  if (received <= 0) {
    return false;
  }
  if (received != size) {
  }

  if (addr) {
    addr->ip = ntohl(srcAddr.sin_addr.s_addr);
    addr->port = ntohs(srcAddr.sin_port);
  }

  return true;
}

} // namespace telemetry_board
