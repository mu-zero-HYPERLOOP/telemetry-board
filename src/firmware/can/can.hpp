#pragma once

#include <cstdint>
namespace telemetry_board {

// This struct is identical to a canzero_frame!
// Hopefully the compiler does a noop on copy or moves.
// NOTE: we don't use canzero_frame here because it
// breaks the dependency order.
// firmware should not depend on CANzero!!!
typedef struct {
  uint32_t id;
  uint8_t dlc;
  uint8_t data[8];
} CanFrame;

bool can0_recv(CanFrame *frame);
bool can1_recv(CanFrame *frame);

bool can0_send(CanFrame *frame);
bool can1_send(CanFrame *frame);

} // namespace telemetry_board
