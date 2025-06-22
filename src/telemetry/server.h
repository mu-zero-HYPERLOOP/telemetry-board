#pragma once

#include "canzero/canzero.h"
#include "telemetry/server_info.hpp"

namespace telemetry::server {

void begin(ServerInfo* serverInfo);

void end();

bool can_recv(uint8_t bus, canzero_frame *frame);
bool can_send(uint8_t bus, canzero_frame *frame);

uint8_t activeConnectionCount();

bool listening();

void update();

} // namespace canzero::telemetry
