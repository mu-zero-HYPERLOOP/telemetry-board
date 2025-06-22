#pragma once

#include "canzero/canzero.h"
#include "telemetry/server_config.h"
#include <array>
#include <cassert>
#include <cstddef>
#include <limits>
#include <optional>

namespace telemetry::server {

class ConnectionIdHost {
  static constexpr uint8_t StaticNodeIdCount =
      static_cast<uint8_t>(node_id_count);
  static_assert(static_cast<size_t>(StaticNodeIdCount) +
                    static_cast<size_t>(MAX_DYNAMIC_IDS) <
                std::numeric_limits<uint8_t>::max());

public:
  ConnectionIdHost() { reset(); }
  ConnectionIdHost(const ConnectionIdHost &) = delete;
  ConnectionIdHost &operator=(const ConnectionIdHost &) = delete;
  ConnectionIdHost(ConnectionIdHost &) = delete;
  ConnectionIdHost &operator=(ConnectionIdHost &) = delete;

  std::optional<uint8_t> alloc_id() {
    if (freeIdsCount == 0) {
      return std::nullopt;
    } else {
      return freeIds[--freeIdsCount];
    }
  }

  void free_id(uint8_t id) {
    assert(freeIdsCount < MAX_DYNAMIC_IDS);
    assert(id >= StaticNodeIdCount && id < StaticNodeIdCount + MAX_DYNAMIC_IDS);
    freeIds[freeIdsCount++] = id;
  }

  void reset() {
    for (uint8_t i = 0; i < MAX_DYNAMIC_IDS; ++i) {
      freeIds[i] = i + StaticNodeIdCount;
    }
    freeIdsCount = MAX_DYNAMIC_IDS;
  }

private:
  std::array<uint8_t, MAX_DYNAMIC_IDS> freeIds;
  std::size_t freeIdsCount;
};

} // namespace telemetry::server
