#include "firmware/telemetry_board.hpp"
#include "util/timestamp.h"
#include "util/timing.h"
#include <chrono>
#include <thread>
#include <utility>

namespace telemetry_board {

constexpr static Duration LinkHeartbeatTimeout = 5_s;
static Timing s_linkHeartbeat;

static LinkStateCallback s_linkStateCallback;

static bool linkStateNotified = true;

void begin() {}

void update() {
  if (linkStateNotified == false && s_linkStateCallback != nullptr) {
    s_linkStateCallback(LinkStateBits::LINK_UP | LinkStateBits::INTERFACE_UP |
                        LinkStateBits::READY);
    linkStateNotified = true;
  }
  delay(1_us);
}

void delay(Duration delta) {
  std::this_thread::sleep_for(
      std::chrono::duration<uint32_t, std::micro>(delta.as_us()));
}
static uint8_t s_macAddress[6];

uint8_t *macAddress() { return s_macAddress; }

uint32_t ipAddress() { return 0; }

LinkStateCallback onLinkStateChange(LinkStateCallback callback) {
  linkStateNotified = false;
  return std::exchange(s_linkStateCallback, callback);
}

void led(bool digital) {}

void lock_irq() {}
void unlock_irq() {}

} // namespace telemetry_board
