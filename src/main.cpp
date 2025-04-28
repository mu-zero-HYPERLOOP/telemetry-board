#include "canzero/canzero.h"
#include "canzero/telemetry/telemetry.h"
#include "util/interval.h"
#include <print>

int main() {

  canzero::telemetry::begin();

  Interval basicInterval{10_Hz};

  while (true) {
    canzero::telemetry::update();

    canzero_frame frame;
    if (canzero::telemetry::can0_recv(&frame)) {
      std::println("can0 frame");
    }

    if (canzero::telemetry::can1_recv(&frame)) {
      std::println("can1 frame");
    }

  }
}
