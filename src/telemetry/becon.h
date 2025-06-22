#pragma once

#include "telemetry/server_info.hpp"
namespace telemetry::becon {

void begin(ServerInfo* serverInfo);

void end();

void update();

}
