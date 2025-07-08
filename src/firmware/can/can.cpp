#include "firmware/can/can.hpp"

#include "./flexcan.h"
#include <cstring>
#include <span>

namespace telemetry_board {

static constexpr bool EnableLoopback = false;

using CANzeroCan0 = Can2;
using CANzeroCan1 = Can3;

void can0_begin(CanBaudrate baudrate, std::span<CanFilter> filters) {
  CanBeginInfo beginInfo;
  beginInfo.baudrate = baudrate;
  beginInfo.loopback = EnableLoopback;
  beginInfo.filters = filters.data();
  beginInfo.filter_count = filters.size();
  CANzeroCan0::begin(&beginInfo);
}

void can1_begin(CanBaudrate baudrate, std::span<CanFilter> filters) {
  CanBeginInfo beginInfo;
  beginInfo.baudrate = baudrate;
  beginInfo.loopback = EnableLoopback;
  beginInfo.filters = filters.data();
  beginInfo.filter_count = filters.size();
  CANzeroCan1::begin(&beginInfo);
}

bool can0_recv(CanFrame *frame) {
  CAN_message_t msg;
  int rx = CANzeroCan0::recv(&msg);
  if (rx) {
    frame->id = msg.id | (msg.flags.extended ? CANZERO_FRAME_IDE_BIT : 0) |
                (msg.flags.remote ? CANZERO_FRAME_RTR_BIT : 0);
    frame->dlc = msg.len;
    std::memcpy(frame->data, msg.buf, frame->dlc * sizeof(uint8_t));
  }
  return rx;
}

bool can1_recv(CanFrame *frame) {
  CAN_message_t msg;
  int rx = CANzeroCan1::recv(&msg);
  if (rx) {
    frame->id = msg.id | (msg.flags.extended ? CANZERO_FRAME_IDE_BIT : 0) |
                (msg.flags.remote ? CANZERO_FRAME_RTR_BIT : 0);
    frame->dlc = msg.len;
    std::memcpy(frame->data, msg.buf, frame->dlc * sizeof(uint8_t));
  }
  return rx;
}

bool can0_send(CanFrame *frame) {
  CAN_message_t msg;
  msg.id = frame->id & 0x1FFFFFFF;
  msg.len = frame->dlc;
  msg.flags.remote = false;
  msg.flags.extended = frame->id & CANZERO_FRAME_IDE_BIT;
  msg.flags.overrun = false;
  std::memcpy(msg.buf, frame->data, frame->dlc * sizeof(uint8_t));
  return CANzeroCan0::send(msg);
}

bool can1_send(CanFrame *frame) {
  CAN_message_t msg;
  msg.id = frame->id & 0x1FFFFFFF;
  msg.len = frame->dlc;
  msg.flags.remote = false;
  msg.flags.extended = frame->id & CANZERO_FRAME_IDE_BIT;
  msg.flags.overrun = false;
  std::memcpy(msg.buf, frame->data, frame->dlc * sizeof(uint8_t));
  return CANzeroCan1::send(msg);
}

} // namespace telemetry_board
