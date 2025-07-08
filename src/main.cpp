#include "bridge.hpp"
#include "can_defaults.h"
#include "canzero/canzero.h"
#include "core_pins.h"
#include "firmware/can/can.hpp"
#include "firmware/telemetry_board.hpp"
#include "pins_arduino.h"
#include "print.h"
#include "telemetry/telemetry.h"
#include "usb_serial.h"
#include "util/interval.h"
#include "util/timing.h"
#include <cassert>


int main() {
  telemetry_board::delay(3_s);


  debugPrintf("Hello, World");

  // start firmware.
  telemetry_board::begin();


  // initalize telemetry (i.e. UDP / TCP servers)
  telemetry::begin();

  // initalize bridge
  // - connect telemetry with the canzero transport layer.
  bridge::begin();

  // initalize canzero.
  // After this call blocking considered an error!
  // - calls into static hooks, which directly initalize physical can
  //   correctly. (These hooks are defined in bridge.cpp)
  canzero_init();
  can_defaults();

  // set can defaults.
  canzero_update_continue(canzero_get_time());



  IntervalTiming loopIntervalTiming;

  while (true) {
    // send canzero_frames
    canzero_update_continue(canzero_get_time());

    // update firmware (i.e. receive from / send on ethernet).
    telemetry_board::update();

    // receive and process incomming packets and datagrams.
    telemetry::update();

    // forward canzero_frames:
    // - from any telemetry connection:
    //   - to the canzero transport layer
    //   - to the physical can.
    // - from the physical can
    //   - to the canzero transport layer
    //   - to all connected telemetry connections.
    bridge::update();

    // canzero transport layer processes incomming canzero_frames
    // (either from physical can or from telemetry).
    canzero_can0_poll();
    canzero_can1_poll();

    loopIntervalTiming.tick();
    canzero_set_loop_frequency(loopIntervalTiming.frequency() / 1_kHz);
         
  }
}
