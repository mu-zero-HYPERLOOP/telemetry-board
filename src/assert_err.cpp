#include "canzero/canzero.h"
#include "core_pins.h"
#include "firmware/telemetry_board.hpp"
#include <cmath>
#include <Arduino.h>
#include <avr/pgmspace.h>

/**
 * This function get's invoked if a assertion fails
 */
void FASTRUN __assert_func(const char *filename, int line, const char *assert_func,
                   const char *expr) {
  while (true) {
    Serial.println("Assertion fault");
    delay(1000);
  }
}
