#pragma once

#include "canzero/canzero.h"
#include <inttypes.h>

namespace telemetry {

void begin();

void update();

bool can_recv(uint8_t bus, canzero_frame *frame);

bool can_send(uint8_t bus, canzero_frame *frame);

} // namespace canzero::telemetry
