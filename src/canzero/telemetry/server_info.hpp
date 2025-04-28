#pragma once

#include "util/timestamp.h"
#include <cstdint>
#include <string_view>
namespace canzero::telemetry { 

struct ServerInfo {
  Timestamp timebase;
  std::uint64_t networkHash;
  std::uint16_t servicePort;
  std::string_view serviceName;
  std::string_view buildTime;
  std::string_view serverName;

};

}
