//
// Created by ranium on 26.06.24.
//

#include "iplayer.h"


IpLayer::IpLayer() :
    stream_(1)
{
}

void IpLayer::create(Network * network, configIp_t * configIp, Time finalSlBearersActivationTime) {
    stream_ += network->getNewRadio()->getNrHelper()->AssignStreams(*network->getUserEquipments()->getUeVoiceNetDev(), stream_);
    stream_ += network->getSidelink()->getNrSlHelper()->AssignStreams(*network->getUserEquipments()->getUeVoiceNetDev(), stream_);
    internetStackHelper_.Install(*network->getUserEquipments()->getUeVoiceContainer());
    stream_ += internetStackHelper_.AssignStreams(*network->getUserEquipments()->getUeVoiceContainer(), stream_);

    groupAddress4_ = Ipv4Address(configIp->groupAddress.c_str());
    remoteAddress_ = InetSocketAddress(groupAddress4_, configIp->port);
    localAddress_ = InetSocketAddress(Ipv4Address::GetAny(), configIp->port);

    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    for (uint32_t u = 0; u < network->getUserEquipments()->getUeVoiceContainer()->GetN(); ++u)
    {
        Ptr<Node> ueNode = network->getUserEquipments()->getUeVoiceContainer()->Get(u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(network->getCore()->getEpcHelper()->GetUeDefaultGatewayAddress(), 1);
    }

    tft_ = Create<LteSlTft>(LteSlTft::Direction::BIDIRECTIONAL, LteSlTft::CommType::GroupCast, groupAddress4_, configIp->dstL2Id);

    network->getSidelink()->getNrSlHelper()->ActivateNrSlBearer(finalSlBearersActivationTime, *network->getUserEquipments()->getUeVoiceNetDev(), tft_);


}




