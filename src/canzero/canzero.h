#ifndef CANZERO_H
#define CANZERO_H
#include <cinttypes>
#include <cstddef>
#define MAX_DYN_HEARTBEATS 10
typedef enum {
  node_id_gamepad = 0,
  node_id_telemetry_board = 1,
  node_id_mother_board = 2,
  node_id_motor_driver = 3,
  node_id_guidance_board_front = 4,
  node_id_guidance_board_back = 5,
  node_id_levitation_board1 = 6,
  node_id_levitation_board2 = 7,
  node_id_levitation_board3 = 8,
  node_id_input_board = 9,
  node_id_power_board12 = 10,
  node_id_power_board24 = 11,
  node_id_led_board = 12,
  node_id_count = 13,
} node_id;
typedef struct {
  uint16_t m_od_index;
  uint8_t m_client_id;
  uint8_t m_server_id;
} get_req_header;
typedef struct {
  uint8_t m_sof;
  uint8_t m_eof;
  uint8_t m_toggle;
  uint16_t m_od_index;
  uint8_t m_client_id;
  uint8_t m_server_id;
} set_req_header;
typedef struct {
  uint8_t m_sof;
  uint8_t m_eof;
  uint8_t m_toggle;
  uint16_t m_od_index;
  uint8_t m_client_id;
  uint8_t m_server_id;
} get_resp_header;
typedef enum {
  set_resp_erno_Success = 0,
  set_resp_erno_Error = 1,
} set_resp_erno;
typedef struct {
  uint16_t m_od_index;
  uint8_t m_client_id;
  uint8_t m_server_id;
  set_resp_erno m_erno;
} set_resp_header;
typedef enum {
  telemetry_state_DISCONNECTED = 0,
  telemetry_state_CONNECTED = 1,
} telemetry_state;
typedef struct {
  uint16_t m_year;
  uint8_t m_month;
  uint8_t m_day;
  uint8_t m_hour;
  uint8_t m_min;
  uint8_t m_sec;
} date_time;
static const node_id CANZERO_NODE_ID = node_id_telemetry_board;
typedef struct {
  uint32_t id;
  uint8_t dlc;
  __attribute__((aligned(alignof(uint64_t)))) uint8_t data[8];
} canzero_frame;
typedef enum : uint32_t {
  CANZERO_FRAME_IDE_BIT = 0x40000000, // 1 << 30
  CANZERO_FRAME_RTR_BIT = 0x80000000, // 1 << 31
} can_frame_id_bits;
typedef struct {
  uint32_t mask;
  uint32_t id;
} canzero_can_filter;
extern void canzero_can0_setup(uint32_t baudrate, canzero_can_filter* filters, int filter_count);
extern void canzero_can0_send(canzero_frame* frame);
extern int canzero_can0_recv(canzero_frame* frame);
extern void canzero_can1_setup(uint32_t baudrate, canzero_can_filter* filters, int filter_count);
extern void canzero_can1_send(canzero_frame* frame);
extern int canzero_can1_recv(canzero_frame* frame);
extern void canzero_request_update(uint32_t time);
extern uint32_t canzero_get_time();
extern void canzero_enter_critical();
extern void canzero_exit_critical();
static inline uint64_t canzero_get_config_hash() {
  extern uint64_t __oe_config_hash;
  return __oe_config_hash;
}
static inline date_time canzero_get_build_time() {
  extern date_time __oe_build_time;
  return __oe_build_time;
}
static inline telemetry_state canzero_get_telemetry_state() {
  extern telemetry_state __oe_telemetry_state;
  return __oe_telemetry_state;
}
static inline uint32_t canzero_get_foo() {
  extern uint32_t __oe_foo;
  return __oe_foo;
}
static inline uint32_t canzero_get_bar() {
  extern uint32_t __oe_bar;
  return __oe_bar;
}
static inline uint32_t canzero_get_baz() {
  extern uint32_t __oe_baz;
  return __oe_baz;
}
static inline uint32_t canzero_get_foobar() {
  extern uint32_t __oe_foobar;
  return __oe_foobar;
}
typedef struct {
  get_resp_header m_header;
  uint32_t m_data;
} canzero_message_get_resp;
static const uint32_t canzero_message_get_resp_id = 0x1BD;
typedef struct {
  set_resp_header m_header;
} canzero_message_set_resp;
static const uint32_t canzero_message_set_resp_id = 0x1DD;
typedef struct {
  telemetry_state m_telemetry_state;
} canzero_message_telemetry_board_stream_state;
static const uint32_t canzero_message_telemetry_board_stream_state_id = 0xD8;
typedef struct {
  uint32_t m_foo;
} canzero_message_telemetry_board_stream_foo;
static const uint32_t canzero_message_telemetry_board_stream_foo_id = 0x98;
typedef struct {
  uint32_t m_bar;
} canzero_message_telemetry_board_stream_bar;
static const uint32_t canzero_message_telemetry_board_stream_bar_id = 0x58;
typedef struct {
  uint32_t m_baz;
} canzero_message_telemetry_board_stream_baz;
static const uint32_t canzero_message_telemetry_board_stream_baz_id = 0x78;
typedef struct {
  uint32_t m_foobar;
} canzero_message_telemetry_board_stream_foobar;
static const uint32_t canzero_message_telemetry_board_stream_foobar_id = 0xB8;
typedef struct {
  uint8_t m_node_id;
  uint8_t m_unregister;
  uint8_t m_ticks_next;
} canzero_message_heartbeat_can0;
static const uint32_t canzero_message_heartbeat_can0_id = 0x1F4;
typedef struct {
  uint8_t m_node_id;
  uint8_t m_unregister;
  uint8_t m_ticks_next;
} canzero_message_heartbeat_can1;
static const uint32_t canzero_message_heartbeat_can1_id = 0x1F3;
typedef struct {
  get_req_header m_header;
} canzero_message_get_req;
static const uint32_t canzero_message_get_req_id = 0x1BE;
typedef struct {
  set_req_header m_header;
  uint32_t m_data;
} canzero_message_set_req;
static const uint32_t canzero_message_set_req_id = 0x1DE;
void canzero_can0_poll();
void canzero_can1_poll();
uint32_t canzero_update_continue(uint32_t delta_time);
void canzero_init();
static inline void canzero_set_config_hash(uint64_t value){
  extern uint64_t __oe_config_hash;
  __oe_config_hash = value;
}

static inline void canzero_set_build_time(date_time value){
  extern date_time __oe_build_time;
  __oe_build_time = value;
}

static inline void canzero_set_telemetry_state(telemetry_state value){
  extern telemetry_state __oe_telemetry_state;
  __oe_telemetry_state = value;
}

static inline void canzero_set_foo(uint32_t value){
  extern uint32_t __oe_foo;
  __oe_foo = value;
}

static inline void canzero_set_bar(uint32_t value){
  extern uint32_t __oe_bar;
  __oe_bar = value;
}

static inline void canzero_set_baz(uint32_t value){
  extern uint32_t __oe_baz;
  __oe_baz = value;
}

static inline void canzero_set_foobar(uint32_t value){
  extern uint32_t __oe_foobar;
  __oe_foobar = value;
}

void canzero_send_config_hash();

void canzero_send_build_time();

void canzero_send_telemetry_state();

void canzero_send_foo();

void canzero_send_bar();

void canzero_send_baz();

void canzero_send_foobar();

#endif