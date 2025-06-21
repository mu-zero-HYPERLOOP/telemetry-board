#include "./bridge.hpp"
#include "can_bus_count.hpp"
#include "print.h"
#include "canzero/canzero.h"
#include "canzero/telemetry/telemetry.h"
#include "firmware/can/can.hpp"
#include "util/cyclic_buffer.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>

namespace bridge {

void begin() {}

static std::array<CyclicBuffer<canzero_frame, 128>, CAN_BUS_COUNT>
    canzero_rxQueues;

static std::array<std::vector<canzero_can_filter>, CAN_BUS_COUNT>
    canzero_can_filters;

template <std::size_t bus>
static bool matches_canzero_filter(canzero_frame *frame) {
  const auto &filters = canzero_can_filters[bus];
  return std::any_of(filters.begin(), filters.end(),
                             [&frame](const canzero_can_filter &filter) {
                               return (frame->id & filter.mask) == filter.id;
                             });
}

template <std::size_t bus>
static bool forward_to_canzero(canzero_frame *frame) {
  if (matches_canzero_filter<bus>(frame)) {
    return canzero_rxQueues[bus].enqueue(*frame);
  }
  return true;
}

template <std::size_t bus>
static bool can_hardware_recv(telemetry_board::CanFrame *canFrame) {
  if constexpr (bus == 0) {
    return telemetry_board::can0_recv(canFrame);
  } else if (bus == 1) {
    return telemetry_board::can0_recv(canFrame);
  }
}

template <std::size_t bus>
static bool can_hardware_send(telemetry_board::CanFrame *canFrame) {
  if constexpr (bus == 0) {
    return telemetry_board::can0_send(canFrame);
  } else if (bus == 1) {
    return telemetry_board::can0_send(canFrame);
  }
}

template <std::size_t bus> static void update_bus() {
  // NOTE: May currently drop frames if the queue capacity is to small.

  telemetry_board::CanFrame canFrame;
  canzero_frame frame;
  // Receive from hardware and forward to
  // - canzero if it matches the filters
  // - forward all frames to the server.
  while (can_hardware_recv<bus>(&canFrame)) {
    frame.id = canFrame.id;
    frame.dlc = canFrame.dlc;
    std::memcpy(frame.data, canFrame.data, sizeof(uint8_t) * 8);
    {
      bool succ = forward_to_canzero<bus>(&frame);
      assert(succ); // TODO REMOVE ASSERTIONS.
    }
    {
      bool succ = canzero::telemetry::can_send(bus, &frame);
      assert(succ);
    }
  }

  // Receive from the server
  // - forward to canzero if it matches the filters.
  // - forward all frames to the hardware can.
  while (canzero::telemetry::can_recv(bus, &frame)) {
    {
      bool succ = forward_to_canzero<bus>(&frame);
      assert(succ);
    }
    canFrame.id = frame.id;
    canFrame.dlc = frame.dlc;
    std::memcpy(canFrame.data, frame.data, sizeof(uint8_t) * 8);
    {
      bool succ = can_hardware_send<bus>(&canFrame);
      assert(succ);
    }
  }
}

template <std::size_t bus_count> static void update_all_buses() {
  if constexpr (bus_count == 0) {
    return;
  } else {
    update_bus<bus_count - 1>();
    update_all_buses<bus_count - 1>();
  }
}

void update() { 
  update_all_buses<CAN_BUS_COUNT>(); 
}

} // namespace bridge

//

//

// ================= CANZERO-HOOKS ================

void canzero_can0_setup(uint32_t baudrate, canzero_can_filter *filters,
                        int filter_count) {

  bridge::canzero_can_filters[0].resize(filter_count);
  std::memcpy(bridge::canzero_can_filters[0].data(), filters,
              sizeof(canzero_can_filter) * filter_count);
  // TODO check if that is a special case.
  telemetry_board::can0_begin(baudrate, {});
}

void canzero_can1_setup(uint32_t baudrate, canzero_can_filter *filters,
                        int filter_count) {

  bridge::canzero_can_filters[1].resize(filter_count);
  std::memcpy(bridge::canzero_can_filters[1].data(), filters,
              sizeof(canzero_can_filter) * filter_count);
  // TODO check if that is a special case.
  telemetry_board::can1_begin(baudrate, {});
}

int canzero_can0_recv(canzero_frame *frame) {
  std::optional<canzero_frame> rx = bridge::canzero_rxQueues[0].dequeue();
  if (rx.has_value()) {
    std::memcpy(frame, &rx.value(), sizeof(canzero_frame));
    return true;
  } else {
    return false;
  }
}

int canzero_can1_recv(canzero_frame *frame) {
  std::optional<canzero_frame> rx = bridge::canzero_rxQueues[1].dequeue();
  if (rx.has_value()) {
    std::memcpy(frame, &rx.value(), sizeof(canzero_frame));
    return true;
  } else {
    return false;
  }
}

void canzero_can0_send(canzero_frame *frame) {
  // Send on the server
  canzero::telemetry::can_send(0, frame);
  telemetry_board::CanFrame canFrame;

  // Send on the hardware can.
  canFrame.id = frame->id;
  canFrame.dlc = frame->dlc;
  std::memcpy(canFrame.data, frame->data, sizeof(uint8_t) * 8);
  telemetry_board::can0_send(&canFrame);
}

void canzero_can1_send(canzero_frame *frame) {
  // Send on the server
  canzero::telemetry::can_send(1, frame);

  // Send on the hardware can.
  telemetry_board::CanFrame canFrame;
  canFrame.id = frame->id;
  canFrame.dlc = frame->dlc;
  std::memcpy(canFrame.data, frame->data, sizeof(uint8_t) * 8);
  telemetry_board::can1_send(&canFrame);
}
