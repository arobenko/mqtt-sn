//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Disconnect.h"
#include <cassert>

namespace mqttsn
{

namespace gateway
{

namespace session_op
{

Disconnect::Disconnect(SessionState& sessionState)
  : Base(sessionState)
{
}

Disconnect::~Disconnect() = default;

void Disconnect::brokerConnectionUpdatedImpl()
{
    auto& st = state();
    if (st.m_brokerConnected || state().m_reconnectingBroker) {
        return;
    }

    if (st.m_connStatus == ConnectionStatus::Asleep) {
        st.m_pendingClientDisconnect = true;
        return;
    }

    if (st.m_connStatus == ConnectionStatus::Connected) {
        sendDisconnectToClient();
    }

    termRequest();
}

void Disconnect::handle(DisconnectMsg_SN& msg)
{
    typedef DisconnectMsg_SN MsgType;
    auto& fields = msg.fields();
    auto& durationField = std::get<MsgType::FieldIdx_duration>(fields);

    if (durationField.getMode() != comms::field::OptionalMode::Missing) {
        // Monotor disconnect with duration in Asleep op
        return;
    }

    sendToBroker(DisconnectMsg());
    sendDisconnectToClient();
    termRequest();
}

void Disconnect::handle(DisconnectMsg& msg)
{
    static_cast<void>(msg);
    if (state().m_connStatus == ConnectionStatus::Connected) {
        sendDisconnectToClient();
    }

    if (state().m_connStatus != ConnectionStatus::Asleep) {
        termRequest();
        return;
    }

    state().m_pendingClientDisconnect = true;
}

void Disconnect::sendDisconnectToClient()
{
    DisconnectMsg_SN respMsg;
    auto& respFields = respMsg.fields();
    auto& durationField = std::get<decltype(respMsg)::FieldIdx_duration>(respFields);
    durationField.setMode(comms::field::OptionalMode::Missing);
    sendToClient(respMsg);
    state().m_connStatus = ConnectionStatus::Disconnected;
}

}  // namespace session_op

}  // namespace gateway

}  // namespace mqttsn



