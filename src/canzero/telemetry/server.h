#pragma once

#include "canzero/canzero.h"
#include "canzero/telemetry/server_info.hpp"
namespace canzero::telemetry::server {

void begin(ServerInfo* serverInfo);

bool can0_recv(canzero_frame *frame);
bool can1_recv(canzero_frame *frame);
bool can0_send(canzero_frame *frame);
bool can1_send(canzero_frame *frame);

void update();

} // namespace canzero::telemetry
