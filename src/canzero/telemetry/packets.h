#pragma once

#include "canzero/canzero.h"
#include <cstdint>
#include <cstring>
namespace canzero::telemetry {

struct HandshakePacket {
  std::uint16_t tag : 4;
  std::uint16_t : 2;
  std::uint16_t sync : 1;
  std::uint16_t reqsuc : 1;
  std::uint16_t node_id : 8;

  static HandshakePacket createOptionRequest(bool sync, bool requestId) {
    HandshakePacket packet{};
    packet.tag = 0x0;
    packet.sync = sync;
    packet.reqsuc = requestId;
    return packet;
  }

  static HandshakePacket createOptionResponse(bool ok, std::uint8_t nodeId) {
    HandshakePacket packet{};
    packet.tag = 0x1;
    packet.reqsuc = ok;
    packet.node_id = nodeId;
    return packet;
  }
};
static_assert(sizeof(HandshakePacket) == 2);

enum class PacketTag : std::uint8_t {
  KeepAlive = 0x0,
  SyncEnd = 0x2,
  NetworkFrame = 0x1,
};

struct __attribute__((packed)) Packet {
  PacketTag tag;
  std::uint8_t bus;
  std::uint8_t dlc;
  std::uint8_t _reserved;
  std::uint32_t canId;
  std::uint64_t timestamp;
  std::uint64_t data;

  static Packet createKeepAlive() {
    Packet packet{};
    packet.tag = PacketTag::KeepAlive;
    return packet;
  }

  static Packet createSyncEnd() {
    Packet packet{};
    packet.tag = PacketTag::SyncEnd;
    return packet;
  }

  static Packet createNetworkFrame(std::uint8_t bus, canzero_frame *frame,
                                   std::uint64_t timestamp) {
    Packet packet{};
    packet.tag = PacketTag::NetworkFrame;
    packet.bus = bus;
    packet.dlc = frame->dlc;
    packet.canId = frame->id;
    packet.timestamp = timestamp;
    std::memcpy(&packet.data, frame->data, sizeof(std::uint64_t));
    return packet;
  }
};

static_assert(sizeof(Packet) == 24);

} // namespace canzero::telemetry
