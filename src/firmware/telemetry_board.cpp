#include "./telemetry_board.hpp"
#include "core_pins.h"
#include "firmware/telemetry/SocketAddr.hpp"
#include "print.h"
#include "util/timestamp.h"
#include <Arduino.h>
#include <QNEthernet.h>

namespace telemetry_board {

static uint8_t s_macAddress[6];
static volatile bool s_linkState;
constexpr static Duration DHCP_timeout = 10_s; // 15 seconds
static uint32_t s_ipAddress;
static IPAddress ip(192, 168, 0, 200);
static IPAddress gateway(192, 168, 0, 1);
static IPAddress subnet(255, 255, 255, 0);

void begin() {


  if (!qindesign::network::Ethernet.begin(ip, subnet, gateway)) {
    debugPrintf("Failed to begin Ethernet\n");
    // TODO error handling. Failed to initalize Ethernet.
  }

  qindesign::network::Ethernet.macAddress(s_macAddress);
  s_linkState = qindesign::network::Ethernet.linkState();

  qindesign::network::Ethernet.onLinkState([](bool state) {
    debugPrintf("LinkState changed\n");
    s_linkState = state;
  });

  // Wait for link state.
  while (!s_linkState) {
    qindesign::network::Ethernet.loop();
  }

  qindesign::network::Ethernet.onInterfaceStatus(
      [](bool x) { debugPrintf("InterfaceStatus changed\n"); });

  qindesign::network::Ethernet.onAddressChanged([]() {
    IPAddress ip = qindesign::network::Ethernet.localIP();
    debugPrintf("[Ethernet] DHCP-renewal. New IP: ");
    printIpAddress(ip);
    debugPrintf("\n");
  });

  IPAddress ip = qindesign::network::Ethernet.localIP();
  debugPrintf("Started Ethernet:\n - MAC: ");
  for (int i = 0; i < 6; ++i) {
    if (i != 0) {
      debugPrintf(":%#02x", s_macAddress[i]);
    } else {
      debugPrintf("%#02x", s_macAddress[i]);
    }
  }
  debugPrintf("\n - IP: ");
  printIpAddress(static_cast<uint32_t>(ip));
  debugPrintf("\n");

  s_ipAddress = static_cast<uint32_t>(ip); // user defined convertion.
}

void update() { qindesign::network::Ethernet.loop(); }

void delay(Duration dur) { delayMicroseconds(dur.as_us()); };

uint8_t *macAddress() { return s_macAddress; }

uint32_t ipAddress() { return s_ipAddress; }

} // namespace telemetry_board
