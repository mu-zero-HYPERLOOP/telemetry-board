#ifndef CANZERO_H
#define CANZERO_H
#include <cinttypes>
#include <cstddef>
#define MAX_DYN_HEARTBEATS 10
typedef enum {
  node_id_gamepad = 0,
  node_id_telemetry_board = 1,
  node_id_motor_driver = 2,
  node_id_input_board = 3,
  node_id_guidance_board_front = 4,
  node_id_guidance_board_back = 5,
  node_id_levitation_board1 = 6,
  node_id_levitation_board2 = 7,
  node_id_levitation_board3 = 8,
  node_id_power_board12 = 9,
  node_id_power_board24 = 10,
  node_id_led_board = 11,
  node_id_count = 12,
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
  telemetry_state_INIT = 0,
  telemetry_state_DISCONNECTED = 1,
  telemetry_state_LISTENING = 2,
  telemetry_state_CLIENT_CONNECTED = 3,
} telemetry_state;
typedef enum {
  bool_t_FALSE = 0,
  bool_t_TRUE = 1,
} bool_t;
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
static inline uint16_t canzero_get_dropped_tx_packets() {
  extern uint16_t __oe_dropped_tx_packets;
  return __oe_dropped_tx_packets;
}
static inline uint16_t canzero_get_dropped_rx_packets() {
  extern uint16_t __oe_dropped_rx_packets;
  return __oe_dropped_rx_packets;
}
static inline uint16_t canzero_get_dropped_tx_canframes() {
  extern uint16_t __oe_dropped_tx_canframes;
  return __oe_dropped_tx_canframes;
}
static inline uint16_t canzero_get_dropped_rx_canframes() {
  extern uint16_t __oe_dropped_rx_canframes;
  return __oe_dropped_rx_canframes;
}
static inline float canzero_get_telemetry_tx_memory_throughput() {
  extern float __oe_telemetry_tx_memory_throughput;
  return __oe_telemetry_tx_memory_throughput;
}
static inline float canzero_get_telemetry_rx_memory_throughput() {
  extern float __oe_telemetry_rx_memory_throughput;
  return __oe_telemetry_rx_memory_throughput;
}
static inline uint8_t canzero_get_active_connections() {
  extern uint8_t __oe_active_connections;
  return __oe_active_connections;
}
static inline bool_t canzero_get_accepts_new_connections() {
  extern bool_t __oe_accepts_new_connections;
  return __oe_accepts_new_connections;
}
static inline float canzero_get_loop_frequency() {
  extern float __oe_loop_frequency;
  return __oe_loop_frequency;
}
typedef struct {
  get_resp_header m_header;
  uint32_t m_data;
} canzero_message_get_resp;
static const uint32_t canzero_message_get_resp_id = 0x13D;
typedef struct {
  set_resp_header m_header;
} canzero_message_set_resp;
static const uint32_t canzero_message_set_resp_id = 0x15D;
typedef struct {
  telemetry_state m_telemetry_state;
  uint8_t m_active_connections;
  bool_t m_accepts_new_connections;
} canzero_message_telemetry_board_stream_state;
static const uint32_t canzero_message_telemetry_board_stream_state_id = 0xD4;
typedef struct {
  float m_telemetry_tx_memory_throughput;
  float m_telemetry_rx_memory_throughput;
} canzero_message_telemetry_board_stream_telemetry_bandwidth;
static const uint32_t canzero_message_telemetry_board_stream_telemetry_bandwidth_id = 0xF4;
typedef struct {
  uint16_t m_dropped_tx_packets;
  uint16_t m_dropped_rx_packets;
  uint16_t m_dropped_tx_canframes;
  uint16_t m_dropped_rx_canframes;
} canzero_message_telemetry_board_stream_dropped_frames;
static const uint32_t canzero_message_telemetry_board_stream_dropped_frames_id = 0x94;
typedef struct {
  float m_loop_frequency;
} canzero_message_telemetry_board_stream_loop_frequency;
static const uint32_t canzero_message_telemetry_board_stream_loop_frequency_id = 0xB4;
typedef struct {
  uint8_t m_node_id;
  uint8_t m_unregister;
  uint8_t m_ticks_next;
} canzero_message_heartbeat_can0;
static const uint32_t canzero_message_heartbeat_can0_id = 0x16F;
typedef struct {
  uint8_t m_node_id;
  uint8_t m_unregister;
  uint8_t m_ticks_next;
} canzero_message_heartbeat_can1;
static const uint32_t canzero_message_heartbeat_can1_id = 0x16E;
typedef struct {
  get_req_header m_header;
} canzero_message_get_req;
static const uint32_t canzero_message_get_req_id = 0x13E;
typedef struct {
  set_req_header m_header;
  uint32_t m_data;
} canzero_message_set_req;
static const uint32_t canzero_message_set_req_id = 0x15E;
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

void canzero_set_telemetry_state(telemetry_state value);

void canzero_set_dropped_tx_packets(uint16_t value);

void canzero_set_dropped_rx_packets(uint16_t value);

void canzero_set_dropped_tx_canframes(uint16_t value);

void canzero_set_dropped_rx_canframes(uint16_t value);

static inline void canzero_set_telemetry_tx_memory_throughput(float value){
  extern float __oe_telemetry_tx_memory_throughput;
  __oe_telemetry_tx_memory_throughput = value;
}

static inline void canzero_set_telemetry_rx_memory_throughput(float value){
  extern float __oe_telemetry_rx_memory_throughput;
  __oe_telemetry_rx_memory_throughput = value;
}

void canzero_set_active_connections(uint8_t value);

void canzero_set_accepts_new_connections(bool_t value);

static inline void canzero_set_loop_frequency(float value){
  extern float __oe_loop_frequency;
  __oe_loop_frequency = value;
}

void canzero_send_config_hash();

void canzero_send_build_time();

void canzero_send_telemetry_state();

void canzero_send_dropped_tx_packets();

void canzero_send_dropped_rx_packets();

void canzero_send_dropped_tx_canframes();

void canzero_send_dropped_rx_canframes();

void canzero_send_telemetry_tx_memory_throughput();

void canzero_send_telemetry_rx_memory_throughput();

void canzero_send_active_connections();

void canzero_send_accepts_new_connections();

void canzero_send_loop_frequency();

#endif