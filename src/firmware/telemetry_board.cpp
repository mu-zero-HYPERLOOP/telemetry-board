#include "./telemetry_board.hpp"
#include "core_pins.h"
#include "firmware/LinkState.hpp"
#include "firmware/telemetry/SocketAddr.hpp"
#include "pins_arduino.h"
#include "print.h"
#include "util/timestamp.h"
#include "util/timing.h"
#include <Arduino.h>
#include <QNEthernet.h>
#include <cassert>

namespace telemetry_board {

static uint8_t s_macAddress[6];
static uint32_t s_ipAddress;
static LinkState s_linkState;

constexpr static Duration LinkHeartbeatTimeout = 5_s;
static Timing s_linkHeartbeat;

static LinkStateCallback s_linkStateCallback;

static void initalizeEthernet() {

  const IPAddress ip(192, 168, 0, 200);
  const IPAddress gateway(192, 168, 0, 1);
  const IPAddress subnet(255, 255, 255, 0);

  s_ipAddress = static_cast<uint32_t>(ip);

  s_linkHeartbeat.start();

  qindesign::network::Ethernet.macAddress(s_macAddress);

  if (qindesign::network::Ethernet.linkState()) {
    s_linkState = LinkStateBits::LINK_UP;
  } else {
    s_linkState = LinkStateBits::LINK_DOWN;
  }

  qindesign::network::Ethernet.onLinkState([](bool state) {
    debugPrintf("Link State changed\n");
    auto prev = s_linkState;
    if (state) {
      s_linkState |= LinkStateBits::LINK_UP;
    } else {
      s_linkState &= ~LinkStateBits::LINK_UP;
    }
    if (s_linkStateCallback && prev != s_linkState) {
      s_linkStateCallback(s_linkState);
    }
  });

  qindesign::network::Ethernet.onInterfaceStatus([](bool state) {
    debugPrintf("Interface Status changed\n");
    auto prev = s_linkState;
    if (state) {
      s_linkState |= LinkStateBits::INTERFACE_UP;
    } else {
      s_linkState &= ~LinkStateBits::INTERFACE_UP;
    }
    if (s_linkStateCallback && prev != s_linkState) {
      s_linkStateCallback(s_linkState);
    }
  });

  qindesign::network::Ethernet.onAddressChanged([]() {
    uint32_t ip = qindesign::network::Ethernet.localIP();
    debugPrintf("Address changed to: ");
    printIpAddress(ip);
    debugPrintf("\n");
    auto prev = s_linkState;
    if (s_ipAddress != ip) {
      s_linkState |= LinkStateBits::ERROR_INET_DOWN;
    } else {
      s_linkState &= ~LinkStateBits::ERROR_INET_DOWN;
    }
    if (s_linkStateCallback && prev != s_linkState) {
      s_linkStateCallback(s_linkState);
    }
  });

  if (!qindesign::network::Ethernet.begin(ip, subnet, gateway)) {
    s_linkState |= LinkStateBits::ERROR_BEGIN_ETHERNET;
  }
  debugPrintf("Initalized ethernet");
}

void begin() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  s_linkStateCallback = nullptr;
  initalizeEthernet();
}

void update() {
  qindesign::network::Ethernet.loop();

  if (((s_linkState & LinkStateBits::READY) != LinkStateBits::READY) ||
      (s_linkState & LinkStateBits::ERROR)) {

    if (s_linkHeartbeat.time() > LinkHeartbeatTimeout) {
      s_linkState |= LinkStateBits::ERROR_LINK_TIMEOUT;
      if (s_linkStateCallback) {
        s_linkStateCallback(s_linkState);
      }
      qindesign::network::Ethernet.end();
      initalizeEthernet();
    }
  } else {
    s_linkHeartbeat.start();
  }
}

void delay(Duration dur) { delayMicroseconds(dur.as_us()); };

uint8_t *macAddress() { return s_macAddress; }

uint32_t ipAddress() {
  if (s_linkState & LinkStateBits::INTERFACE_UP) {
    return s_ipAddress;
  } else {
    return 0;
  }
}

LinkStateCallback onLinkStateChange(LinkStateCallback callback) {
  return std::exchange(s_linkStateCallback, callback);
}

void led(bool digital) {
  digitalWrite(LED_BUILTIN, static_cast<uint8_t>(digital));
}

} // namespace telemetry_board
