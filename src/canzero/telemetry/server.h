#pragma once

#include "canzero/canzero.h"
#include "canzero/telemetry/server_info.hpp"
namespace canzero::telemetry::server {

void begin(ServerInfo* serverInfo);

bool can_recv(uint8_t bus, canzero_frame *frame);
bool can_send(uint8_t bus, canzero_frame *frame);

void update();

} // namespace canzero::telemetry
