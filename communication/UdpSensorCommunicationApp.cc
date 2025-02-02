//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "UdpSensorCommunicationApp.h"
#include "MobileNodeMessage_m.h"
#include "inet/applications/base/ApplicationPacket_m.h"
#include "UdpBasicAppMobileSensorNode.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "MobileSensorNode.h"

namespace projeto {

Define_Module(UdpSensorCommunicationApp);

void UdpSensorCommunicationApp::sendPacket() {
        Packet *packet = new Packet("SensorMessage");
        if(dontFragment)
            packet->addTag<FragmentationReq>()->setDontFragment(true);
        packet->setName(this->getParentModule()->getFullName());
        const auto& payload = makeShared<MobileNodeMessage>();
        payload->addTag<CreationTimeTag>()->setCreationTime(simTime());

        payload->setMessageType(MessageType::BEARER);
        payload->setSourceID(this->getParentModule()->getIndex());

        packet->insertAtBack(payload);
        L3Address destAddr = chooseDestAddr();

        emit(packetSentSignal, packet);
        socket.sendTo(packet, destAddr, destPort);
        numSent++;
}

} /* namespace projeto */
