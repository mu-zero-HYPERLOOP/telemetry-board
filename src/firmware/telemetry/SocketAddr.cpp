#include "./SocketAddr.hpp"
#include "print.h"

void telemetry_board::printIpAddress(unsigned int ip) {
  unsigned int a1 = ip & 0xFF;
  unsigned int a2 = (ip >> 8) & 0xFF;
  unsigned int a3 = (ip >> 16) & 0xFF;
  unsigned int a4 = (ip >> 24) & 0xFF;

  debugPrintf("%d.%d.%d.%d", a1, a2, a3, a4);
}
void telemetry_board::printSocketAddress(const telemetry_board::SocketAddr& addr) {
  printIpAddress(addr.ip);
  debugPrintf(":%d", addr.port);
}

