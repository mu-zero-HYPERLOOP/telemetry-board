#pragma once

#include <cstdint>
#include <string_view>

namespace canzero::telemetry {

enum class DatagramTag : std::uint8_t{
  OptionRequest = 0x0,
  OptionResponse = 0x1,
};

struct Datagram {
  DatagramTag tag;
  std::uint8_t _reserved;
  std::uint16_t servicePort;
  std::uint32_t _reserved_;
  std::uint64_t networkHash;
  std::uint8_t serviceNameLen;
  char serviceNameBuffer[63];

  std::uint8_t buildTimeLen;
  char buildTimeBuffer[63];

  std::uint8_t serverNameLen;
  char serverNameBuffer[63];


  std::string_view serviceName() const {
    return std::string_view(serviceNameBuffer, static_cast<std::size_t>(serviceNameLen));
  }

  std::string_view buildTime() const {
    return std::string_view(buildTimeBuffer, static_cast<std::size_t>(buildTimeLen));
  }

  std::string_view serverName() const {
    return std::string_view(serverNameBuffer, static_cast<std::size_t>(serverNameLen));
  }
};

}
