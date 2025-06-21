#include "firmware/can/can.hpp"

namespace telemetry_board {

void can0_begin(std::uint32_t baudrate, std::span<CanFilter> filters) {

}

void can1_begin(std::uint32_t baudrate, std::span<CanFilter> filters) {

}

bool can0_recv(CanFrame *frame) { return false; }

bool can1_recv(CanFrame *frame) { return false; }

bool can0_send(CanFrame *frame) { return true; }

bool can1_send(CanFrame *frame) { return true; }

} // namespace telemetry_board
