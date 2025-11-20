#include "firmware/telemetry_board.hpp"
#include "print.h"
#include <avr/pgmspace.h>
#include <cmath>

/**
 * This function get's invoked if a assertion fails
 */
void FASTRUN __assert_func(const char *filename, int line,
                           const char *assert_func, const char *expr) {
  while (true) {
    debugPrintf("Assertion Fault");
    telemetry_board::delay(1000_ms);
  }
}
