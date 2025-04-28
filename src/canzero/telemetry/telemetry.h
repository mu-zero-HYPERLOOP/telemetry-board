#pragma once

#include "canzero/canzero.h"
namespace canzero::telemetry {

void begin();

void update();

bool can0_recv(canzero_frame *frame);
bool can1_recv(canzero_frame *frame);

void can0_send(canzero_frame *frame);
void can1_send(canzero_frame *frame);

} // namespace canzero::telemetry
