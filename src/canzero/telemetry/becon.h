#pragma once

#include "canzero/telemetry/server_info.hpp"
namespace canzero::telemetry::becon {

void begin(ServerInfo* serverInfo);

void update();

}
