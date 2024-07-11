//
// Created by ranium on 21.06.24.
//

#include "utils.h"

using namespace ns3;

void Utils::packetClientRx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
    std::cout << "Packet client Rx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
}

void Utils::packetClientTx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
    std::cout << "Packet client Tx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
}

void Utils::packetServerRx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
    std::cout << "Packet server Rx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
}

void Utils::packetClientIpv4L3Protocol(const Ipv4Header& header, Ptr<const Packet> packet, uint32_t interface)
{
    std::cout << "Packet client Ipv4 L3 Protocol: " << packet->GetSize() << " bytes from " << header.GetSource() << " to " << header.GetDestination() << " on interface " << interface << std::endl;
}

void Utils::packetIpForwardUnicast(const Ipv4Header &header, Ptr< const Packet > packet, uint32_t interface)
{
    std::cout << "Packet IP Forward Unicast: " << packet->GetSize() << " bytes from " << header.GetSource() << " to " << header.GetDestination() << " on interface " << interface << " with a TTL of " << header.GetTtl() << std::endl;
}
