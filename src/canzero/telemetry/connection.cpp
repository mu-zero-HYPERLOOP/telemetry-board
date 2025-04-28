#include "./connection.h"
#include <print>

namespace canzero::telemetry {

bool Connection::recv(canzero_frame *frame) {
  while (true) {
    switch (m_state) {
    case ConnectionState::Closed:
      return false;
    case ConnectionState::WaitingForOptionRequest: {
      HandshakePacket optionRequest;
      //
      switch (m_tcp.recv(&optionRequest, sizeof(optionRequest))) {
      case telemetry_board::TcpRecvInfo::CLOSED:
        close();
        return false;
      case telemetry_board::TcpRecvInfo::EMPTY:
        return false;
      case telemetry_board::TcpRecvInfo::SUCC:
        if (optionRequest.tag == 0x0) { // Option Request
          if (optionRequest.sync == false && optionRequest.reqsuc == false) {
            // NOTE: Spec states that no response should be send as it is
            // implied that all canzero telemetry servers should support the
            // default communication pattern.
            m_state = ConnectionState::Communication;
          } else {
            // Handle option requests
            bool ok = true;
            std::uint8_t nodeId;
            if (optionRequest.sync == true) {
              ok = false;
            }
            if (optionRequest.reqsuc == true) {
              ok = false;
            }
            m_optionResponse =
                HandshakePacket::createOptionResponse(ok, nodeId);
            const auto r =
                m_tcp.send(&m_optionResponse, sizeof(m_optionResponse));
            switch (r) {
            case telemetry_board::TcpSendInfo::CLOSED:
              close();
              break;
            case telemetry_board::TcpSendInfo::SKIPPED:
              m_state = ConnectionState::ReceivedOptionRequest;
              break;
            case telemetry_board::TcpSendInfo::SUCC:
              m_state = ConnectionState::Communication;
              break;
            }
          }
        }
      }
      continue;
    }
    case ConnectionState::ReceivedOptionRequest: {
      const auto r = m_tcp.send(&m_optionResponse, sizeof(m_optionResponse));
      switch (r) {
      case telemetry_board::TcpSendInfo::CLOSED:
        close();
        return false;
      case telemetry_board::TcpSendInfo::SUCC:
        m_state = ConnectionState::Communication;
        continue;
      case telemetry_board::TcpSendInfo::SKIPPED:
        return false;
      }
    }
    case ConnectionState::Communication: {
      //
      Packet packet;
      auto r = m_tcp.recv(&packet, sizeof(packet));
      switch (r) {
      case telemetry_board::TcpRecvInfo::CLOSED:
        close();
        return false;
      case telemetry_board::TcpRecvInfo::EMPTY:
        return false;
      case telemetry_board::TcpRecvInfo::SUCC:
        switch (packet.tag) {
        case PacketTag::KeepAlive:
          continue;
        case PacketTag::SyncEnd:
          std::println("Received unexpected SyncEnd frame");
          close();
          return false;
        [[likely]] case PacketTag::NetworkFrame:
          frame->id = packet.canId;
          frame->dlc = packet.dlc;
          std::memcpy(frame->data, &packet.data, sizeof(std::uint64_t));
          continue;
        }
      }
    }
    }
  }
}

bool Connection::send(std::uint8_t bus, canzero_frame *frame,
                      std::uint64_t timestamp) {
  if (m_state == ConnectionState::Communication) {
    Packet packet = Packet::createNetworkFrame(bus, frame, timestamp);
    auto r = m_tcp.send(&packet, sizeof(packet));
    switch (r) {
    case telemetry_board::TcpSendInfo::CLOSED:
      close();
      break;
    case telemetry_board::TcpSendInfo::SKIPPED:
      break;
    case telemetry_board::TcpSendInfo::SUCC:
      return true;
    }
  }
  return false;
}
void Connection::close() {
  m_tcp.close();
  m_state = ConnectionState::Closed;
}
} // namespace canzero::telemetry
