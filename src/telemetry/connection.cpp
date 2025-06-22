#include "./connection.h"
#include "telemetry/packets.h"
#include "firmware/telemetry/TcpServer.hpp"
#include "print.h"
#include "util/timing.h"

namespace telemetry {

bool Connection::recv(Packet *packet) {
  auto opt = m_rxQueue.dequeue();
  if (opt.has_value()) {
    *packet = opt.value();
    return true;
  }
  return false;
}

bool Connection::send(const Packet *packet) {
  [[likely]] if (m_state == ConnectionState::Communication) {
    const auto r = m_tcp.send_exact(packet, sizeof(*packet));
    switch (r) {
    case telemetry_board::TcpSendResult::CLOSED:
      close();
      return false;
    case telemetry_board::TcpSendResult::WOULD_BLOCK:
      m_txQueue.enqueue(*packet);
    case telemetry_board::TcpSendResult::SUCC:
      return true;
    }
  } else {
    return false;
  }
}

void Connection::update() {
  // Send packets (only includes packets that where previously SKIPPED)
  {
    int count = 0;
    Timing timing;
    timing.start();
    std::optional<Packet> packet;
    bool con = true;

    while (con && (packet = m_txQueue.peek()).has_value()) {
      count += 1;
      switch (m_tcp.send_exact(&packet.value(), sizeof(packet.value()))) {
      case telemetry_board::TcpSendResult::CLOSED:
        close();
      case telemetry_board::TcpSendResult::WOULD_BLOCK:
        con = false;
        break;
      case telemetry_board::TcpSendResult::SUCC:
        (void)m_txQueue.dequeue();
        break;
      }
    }
    if (timing.time() > 1_ms) {
      debugPrintf(
          "Connection::update processing tx-queue took %dus (%d packets)\n",
          timing.time().as_us(), count);
    }
  }
  if (m_keepAliveInterval.peekNext()) {
    auto packet = Packet::createKeepAlive();
    switch (m_tcp.send_exact(&packet, sizeof(packet))) {
    case telemetry_board::TcpSendResult::CLOSED:
      close();
    case telemetry_board::TcpSendResult::WOULD_BLOCK:
      break;
    case telemetry_board::TcpSendResult::SUCC:
      m_keepAliveInterval.next();
      break;
    }
  }

  // Receive packets
  while (m_rxQueue.hasSpace()) {
    [[likely]] if (m_state == ConnectionState::Communication) {
      Packet packet;
      auto r = m_tcp.recv_exact(&packet, sizeof(packet));
      if (r == telemetry_board::TcpRecvResult::SUCC) {
        [[likely]] if (packet.tag == PacketTag::NetworkFrame) {
          m_rxQueue.enqueue(packet);
        }
      } else {
        if (r == telemetry_board::TcpRecvResult::CLOSED) {
          close();
        }
        break;
      }
      continue;
    }

    if (m_state == ConnectionState::Closed) {
      break;
    }
    if (m_state == ConnectionState::WaitingForOptionRequest) {
      HandshakePacket optionRequest;
      auto r = m_tcp.recv_exact(&optionRequest, sizeof(optionRequest));
      [[likely]] if (r == telemetry_board::TcpRecvResult::SUCC) {
        [[likely]] if (optionRequest.tag == 0x0) { // is option request
          debugPrintf(
              "[TCP-Server] Received OptionRequest: {sync=%d, idreq=%d}\n",
              optionRequest.sync, optionRequest.reqsuc);
          if (optionRequest.sync == false && optionRequest.reqsuc == false) {
            // NOTE: Edgecase. If no options are requested we don't have to
            // send a option response because success is implied.
            debugPrintf("[TCP-Server] -> accepted connection (no-response)\n");
            m_state = ConnectionState::Communication;
          } else {
            bool ok = true;
            std::uint8_t nodeId = 0;
            if (optionRequest.sync == true) {
              ok = false;
            }
            if (optionRequest.reqsuc == true) {
              std::optional<uint8_t> nodeIdreq = m_idHost->alloc_id();
              if (nodeIdreq.has_value()) {
                nodeId = nodeIdreq.value();
                m_uniqueId = nodeId;
              } else {
                ok = false;
              }
            }
            if (ok) {
              debugPrintf("[TCP-Server] -> accepted connection\n");
            } else {
              debugPrintf("[TCP-Server] -> refused connection\n");
            }
            m_optionResponse =
                HandshakePacket::createOptionResponse(ok, nodeId);
            auto r =
                m_tcp.send_exact(&m_optionResponse, sizeof(m_optionResponse));
            [[likely]] if (r == telemetry_board::TcpSendResult::SUCC) {
              m_state = ConnectionState::Communication;
            } else {
              if (r == telemetry_board::TcpSendResult::CLOSED) {
                close();
                break;
              } else {
                m_state = ConnectionState::ReceivedOptionRequest;
                break;
              }
            }
          }
        }
      } else {
        if (r == telemetry_board::TcpRecvResult::CLOSED) {
          close();
        }
        return;
      }
      continue;
    }

    if (m_state == ConnectionState::ReceivedOptionRequest) {
      const auto r =
          m_tcp.send_exact(&m_optionResponse, sizeof(m_optionResponse));
      switch (r) {
      case telemetry_board::TcpSendResult::CLOSED:
        close();
        break;
      case telemetry_board::TcpSendResult::WOULD_BLOCK:
        // retry next time
        break;
      case telemetry_board::TcpSendResult::SUCC:
        m_state = ConnectionState::Communication;
      }
    }
  }
  if (!m_rxQueue.hasSpace()) {
    debugPrintf("Connection blocked by rxQueue\n");
  }
}

void Connection::close() {
  if (m_state != ConnectionState::Closed) {
    m_tcp.close();
    m_state = ConnectionState::Closed;
  }
}
} // namespace canzero::telemetry
