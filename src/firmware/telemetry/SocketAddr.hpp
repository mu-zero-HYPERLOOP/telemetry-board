#pragma once

#include <cstdint>
#include "print.h"
namespace telemetry_board {

struct SocketAddr {
  unsigned int ip;
  std::uint16_t port;
};

void printIpAddress(unsigned int ip);

void printSocketAddress(const SocketAddr& ip);
}
