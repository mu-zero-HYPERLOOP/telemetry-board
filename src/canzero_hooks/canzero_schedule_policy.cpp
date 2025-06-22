#include "avr/pgmspace.h"
#include "canzero/canzero.h"
#include "util/timestamp.h"

static Timestamp g_start = Timestamp::now();

uint32_t FASTRUN canzero_get_time() {
  return (Timestamp::now() - g_start).as_ms();
}

void FASTRUN canzero_request_update(uint32_t time) {}

void canzero_exit_critical() {}

void canzero_enter_critical() {}
