#pragma once

#include <cstdint>
namespace telemetry_board {

struct SocketAddr {
  unsigned int ip;
  std::uint16_t port;
};
}
