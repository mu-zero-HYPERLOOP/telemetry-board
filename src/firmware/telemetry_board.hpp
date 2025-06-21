#pragma once

#include "util/timestamp.h"
namespace telemetry_board {

void begin();

void update();

void delay(Duration dur);

uint8_t *macAddress();

uint32_t ipAddress();

} // namespace telemetry_board
