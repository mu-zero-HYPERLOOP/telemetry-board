#pragma once
#include "canzero/canzero.h"
#include "print.h"
#include <avr/pgmspace.h>
#include <cassert>
#include <span>

namespace telemetry_board {

enum CanBaudrate : uint32_t {
  CAN_BAUDRATE_125Kbps = 125000,
  CAN_BAUDRATE_250Kbps = 250000,
  CAN_BAUDRATE_500Kbps = 500000,
  CAN_BAUDRATE_1000Kbps = 1000000,
};

inline CanBaudrate baudrateFromU32(uint32_t baudrate) {
  if (baudrate == telemetry_board::CanBaudrate::CAN_BAUDRATE_125Kbps) {
    return telemetry_board::CanBaudrate::CAN_BAUDRATE_125Kbps;
  } else if (baudrate == telemetry_board::CanBaudrate::CAN_BAUDRATE_250Kbps) {
    return telemetry_board::CanBaudrate::CAN_BAUDRATE_250Kbps;
  } else if (baudrate == telemetry_board::CanBaudrate::CAN_BAUDRATE_500Kbps) {
    return telemetry_board::CanBaudrate::CAN_BAUDRATE_500Kbps;
  } else if (baudrate == telemetry_board::CanBaudrate::CAN_BAUDRATE_1000Kbps) {
    return telemetry_board::CanBaudrate::CAN_BAUDRATE_1000Kbps;
  } else {
    debugPrintf("Not a valid CAN baudrate %d\n", baudrate);
    assert(false);
    return static_cast<CanBaudrate>(0);
  }
}

struct CanFilter {
  uint32_t id;
  uint32_t mask;
  bool ide;
};

struct CanFrame{
  uint32_t id;
  uint8_t dlc;
  __attribute__((aligned(alignof(uint64_t)))) uint8_t data[8];
};

void can0_begin(CanBaudrate baudrate, std::span<CanFilter> filters);
void can1_begin(CanBaudrate baudrate, std::span<CanFilter> filters);

bool can0_recv(CanFrame *frame);
bool can1_recv(CanFrame *frame);

bool can0_send(CanFrame *frame);
bool can1_send(CanFrame *frame);

// bool can0_recv(CAN_frea



} // namespace telemetry_board
