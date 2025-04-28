#pragma once

#include "firmware/telemetry/SocketAddr.hpp"
#include <cstdint>
namespace telemetry_board {

class UdpServer {
  public:
    explicit UdpServer(std::size_t maxDatagramSize);
    ~UdpServer();

    void connect(std::uint16_t port);

    bool send(SocketAddr addr, const void* data, std::size_t size);

    bool recv(void* data, std::size_t size, SocketAddr* addr);

  
  private:
    // TODO keep local receive buffer
    void* m_internals;
};

}
