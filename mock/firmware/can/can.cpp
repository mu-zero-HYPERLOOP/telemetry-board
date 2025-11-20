#include "firmware/can/can.hpp"
#include "firmware/can/tcpcan.h"

namespace telemetry_board {

static std::unique_ptr<tcpcan::TcpCan<2>> s_tcpcan = nullptr;

void can0_begin(CanBaudrate baudrate, std::span<CanFilter> filters) {
  if (s_tcpcan.get() == nullptr) {
    s_tcpcan = tcpcan::TcpCan<2>::connect();
  }
}

void can1_begin(CanBaudrate baudrate, std::span<CanFilter> filters) {
  if (s_tcpcan.get() == nullptr) {
    s_tcpcan = tcpcan::TcpCan<2>::connect();
  }
}

bool can0_recv(CanFrame *frame) {
  std::optional<tcpcan::CanFrame> rx = s_tcpcan->recv<0>();
  if (!rx.has_value()) {
    return 0;
  }
  frame->dlc = rx->dlc;
  frame->id = rx->can_id;
  *reinterpret_cast<uint64_t *>(frame->data) = rx->data;
  return 1;
}

bool can1_recv(CanFrame *frame) {
  std::optional<tcpcan::CanFrame> rx = s_tcpcan->recv<1>();
  if (!rx.has_value()) {
    return 0;
  }
  frame->dlc = rx->dlc;
  frame->id = rx->can_id;
  *reinterpret_cast<uint64_t *>(frame->data) = rx->data;
  return 1;
}

bool can0_send(CanFrame *frame) {
  tcpcan::CanFrame can_frame(
      frame->id, *reinterpret_cast<uint64_t *>(frame->data), frame->dlc);
  s_tcpcan->send<0>(can_frame);
  return true;
}

bool can1_send(CanFrame *frame) {
  tcpcan::CanFrame can_frame(
      frame->id, *reinterpret_cast<uint64_t *>(frame->data), frame->dlc);
  s_tcpcan->send<1>(can_frame);
  return true;
}

} // namespace telemetry_board
