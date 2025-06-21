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
  src/main.cpp
  src/canzero/telemetry/telemetry.cpp
  src/canzero/telemetry/becon.cpp
  src/canzero/telemetry/server.cpp
  src/canzero/telemetry/connection.cpp
  src/canzero/canzero.cpp
  src/canzero/canzero_schedule_policy_hooks.cpp
  src/bridge.cpp
  src/util/build_time.cpp

  mock/firmware/can/can.cpp
  mock/firmware/telemetry/UdpServer.cpp
  mock/firmware/telemetry/TcpServer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src/)
target_include_directories(${PROJECT_NAME} PRIVATE mock/)



