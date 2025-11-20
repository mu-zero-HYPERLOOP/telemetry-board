#pragma once

#include "firmware/LinkState.hpp"
#include "util/timestamp.h"

namespace telemetry_board {

typedef void (*LinkStateCallback)(LinkState);

void begin();

void update();

void delay(Duration dur);

uint8_t *macAddress();

uint32_t ipAddress();

LinkStateCallback onLinkStateChange(LinkStateCallback callback);

void led(bool digital);

void lock_irq();
void unlock_irq();

} // namespace telemetry_board
