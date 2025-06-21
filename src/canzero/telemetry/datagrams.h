#pragma once

#include "canzero/telemetry/server_info.hpp"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <string_view>

namespace canzero::telemetry {

enum class DatagramTag : std::uint8_t {
  Hello = 0x0,
  NetworkDescription = 0x1,
};

struct Datagram {
  DatagramTag tag;
  std::uint8_t _reserved;
  std::uint16_t servicePort;
  std::uint32_t _reserved_;
  std::uint64_t networkHash;
  std::uint64_t timebase;
  std::uint8_t serviceNameLen;
  char serviceNameBuffer[63];

  std::uint8_t buildTimeLen;
  char buildTimeBuffer[63];

  std::uint8_t serverNameLen;
  char serverNameBuffer[63];

  std::string_view serviceName() const {
    return std::string_view(serviceNameBuffer,
                            static_cast<std::size_t>(serviceNameLen));
  }

  std::string_view buildTime() const {
    return std::string_view(buildTimeBuffer,
                            static_cast<std::size_t>(buildTimeLen));
  }

  std::string_view serverName() const {
    return std::string_view(serverNameBuffer,
                            static_cast<std::size_t>(serverNameLen));
  }

  static Datagram createHello(std::string_view serviceName) {
    Datagram datagram{};
    datagram.tag = DatagramTag::Hello;
    {
      assert(serviceName.size() + 1 <= 63);
      std::memcpy(datagram.serviceNameBuffer, serviceName.data(),
                  serviceName.size());
    }
    return datagram;
  }

  static Datagram createNetworkDescription(std::uint16_t servicePort,
                                           std::uint64_t networkHash,
                                           std::uint64_t timebase,
                                           std::string_view serviceName,
                                           std::string_view buildTime,
                                           std::string_view serverName) {
    Datagram datagram{};
    datagram.tag = DatagramTag::NetworkDescription;
    datagram.servicePort = servicePort;
    datagram.networkHash = networkHash;
    datagram.timebase = timebase;
    {
      assert(serviceName.size() + 1 <= 63);
      datagram.serviceNameLen = serviceName.size();
      std::memcpy(datagram.serviceNameBuffer, serviceName.data(),
                  serviceName.size());
    }

    {
      assert(buildTime.size() + 1 <= 63);
      datagram.buildTimeLen = buildTime.size();
      std::memcpy(datagram.buildTimeBuffer, buildTime.data(), buildTime.size());
    }

    {
      assert(serverName.size() + 1 <= 63);
      datagram.serverNameLen = serverName.size();
      std::memcpy(datagram.serverNameBuffer, serverName.data(),
                  serverName.size());
    }
    return datagram;
  }

  static Datagram createNetworkDescription(ServerInfo *serverInfo) {
    return createNetworkDescription(
        serverInfo->servicePort, serverInfo->networkHash,
        (Timestamp::now() - serverInfo->timebase).as_us(),
        serverInfo->serviceName, serverInfo->buildTime, serverInfo->serverName);
  }
};

static_assert(sizeof(Datagram) == 216);

} // namespace canzero::telemetry
