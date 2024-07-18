//
// Created by ranium on 21.06.24.
//

#include "utils.h"

using namespace ns3;

uint32_t Utils::packetSent(0);
uint32_t Utils::packetReceived(0);

void Utils::packetClientRx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    uint32_t p_id = packet->GetUid();
    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
    std::cout << "Packet (" << p_id << ") client Rx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;

}

void Utils::packetClientTx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    uint32_t p_id = packet->GetUid();
    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
    std::cout << "Packet (" << p_id << ") client Tx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
    packetSent++;
}

void Utils::packetServerRx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    uint32_t p_id = packet->GetUid();
    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
    std::cout << "Packet (" << p_id << ") server Rx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
    packetReceived++;
}

void Utils::packetClientIpv4L3Protocol(const Ipv4Header& header, Ptr<const Packet> packet, uint32_t interface)
{
    uint32_t p_id = header.GetIdentification();
    std::cout << "Packet (" << p_id << ") client Ipv4 L3 Protocol: " << packet->GetSize() << " bytes from " << header.GetSource() << " to " << header.GetDestination() << " on interface " << interface << std::endl;
}

void Utils::packetIpForwardUnicast(const Ipv4Header &header, Ptr< const Packet > packet, uint32_t interface)
{
    uint32_t ttl = header.GetTtl();
    uint32_t p_id = packet->GetUid();
    std::cout << "Packet (" << p_id << ") IP Forward Unicast: " << packet->GetSize() << " bytes from " << header.GetSource() << " to " << header.GetDestination() << " on interface " << interface << " with a TTL of " << ttl << std::endl;
}

void Utils::ipv4ProtocolSendRealOut(Ptr<Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface)
{
    uint32_t p_id = packet->GetUid();
    Ipv4Header header;
    packet->PeekHeader(header);
    std::cout << "Packet (" << p_id << ") Ipv4 Protocol Send Real Out: " << packet->GetSize() << " bytes on interface " << interface << std::endl;
    std::cout << "  ipv4 : " << ipv4->GetAddress(interface, 0).GetLocal() << std::endl;
    std::cout << "  src : " << header.GetSource() << " dest : " << header.GetDestination() << std::endl;

}
