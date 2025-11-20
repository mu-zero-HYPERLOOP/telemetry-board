######################################################################
# @author      : karl (karlsasssie@gmail.com)
# @file        : CMakeLists
# @created     : Friday Apr 07, 2023 15:39:05 CEST
######################################################################

cmake_minimum_required(VERSION 3.5)
project(telemetry-board CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
  src/canzero/canzero.cpp
  src/canzero_hooks/canzero_schedule_policy.cpp
  mock/firmware/can/can.cpp
  mock/firmware/telemetry/TcpServer.cpp
  mock/firmware/telemetry/UdpServer.cpp
  mock/firmware/telemetry_board.cpp
  src/firmware/telemetry/SocketAddr.cpp
  src/telemetry/becon.cpp
  src/telemetry/connection.cpp
  src/telemetry/server.cpp
  src/telemetry/telemetry.cpp
  src/util/build_time.cpp
  src/assert_err.cpp
  src/bridge.cpp
  src/main.cpp
  mock/print.cpp

)

target_include_directories(${PROJECT_NAME} PRIVATE src/)
target_include_directories(${PROJECT_NAME} PRIVATE mock/)



