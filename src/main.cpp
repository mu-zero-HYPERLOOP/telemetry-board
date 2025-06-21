#include "bridge.hpp"
#include "canzero/canzero.h"
#include "canzero/telemetry/telemetry.h"
#include "firmware/telemetry_board.hpp"
#include "print.h"
#include "util/interval.h"
#include "util/timing.h"
#include <Arduino.h>
#include <cassert>

int main() {
  Serial.begin(9600);

  telemetry_board::delay(3_s);

  telemetry_board::begin();

  bridge::begin();

  canzero::telemetry::begin();

  IntervalTiming loopItervalTimer;
  Interval logInterval{10_s};

  canzero_init();
  canzero_update_continue(canzero_get_time());

  while (true) {
    Timing timing;
    timing.start();

    canzero_can0_poll();
    canzero_can1_poll();

    {
      Timing timing;
      timing.start();
      telemetry_board::update();

      if (timing.time() > 1_ms) {
        debugPrintf("firmware::update took %dus\n", timing.time().as_us());
      }
    }
    {
      Timing timing;
      timing.start();
      canzero::telemetry::update();
      if (timing.time() > 1_ms) {
        debugPrintf("canzero::telemetry::update took %dus\n",
                    timing.time().as_us());
      }
    }
    {
      Timing timing;
      timing.start();
      bridge::update();
      if (timing.time() > 1_ms) {
        debugPrintf("bridge::update took %dus\n", timing.time().as_us());
      }

    }

    canzero_update_continue(canzero_get_time());

    loopItervalTimer.tick();

    if (logInterval.next()) {
      debugPrintf("Loop-Interval: %fHz\n",
                  static_cast<float>(loopItervalTimer.frequency()));
    }

    if (timing.time() > 1_ms) {
      debugPrintf("Slow loop : %fms", timing.time().as_us() * 1e-3);
      assert(false);
    }
  }
}
