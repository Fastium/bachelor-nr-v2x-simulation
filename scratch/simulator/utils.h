//
// Created by ranium on 21.06.24.
//

#ifndef BACHELOR_NS3_SIMULATOR_UTILS_HPP
#define BACHELOR_NS3_SIMULATOR_UTILS_HPP

#include "ns3/antenna-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/config-store.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/log.h"
#include "ns3/lte-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/nr-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/stats-module.h"

#include "packetoutputdb.h"

using namespace ns3;

class Utils {
public:
    static void packetClientTx(const Ptr< const Packet > packet, const Address &srcAddress, const Address &destAddress);

    static void packetServerRx(const Ptr< const Packet > packet, const Address &srcAddress, const Address &destAddress);

    static void packetServerRxWithHeader(Ptr< const Packet > p, const Address &from, const Address &to, const SeqTsSizeHeader &header);

    static void packetClientRx(const Ptr< const Packet > packet, const Address &srcAddress, const Address &destAddress);

    static void packetClientIpv4L3Protocol(const Ipv4Header &header, Ptr< const Packet > packet, uint32_t interface);

    static void packetIpForwardUnicast(const Ipv4Header &header, Ptr< const Packet > packet, uint32_t interface);

    static void ipv4ProtocolSendRealOut(Ptr<Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface);

    static void ipv4Receive(Ptr< const Packet > packet, Ptr< Ipv4 > ipv4, uint32_t interface);

    static void notifyPacket(PacketOutputDb packetOutputDb, const Ptr< const Packet > packet, const Address &srcAddress, const Address &destAddress);


    static uint32_t packetSent;
    static uint32_t packetReceived;
    static uint32_t packetReceivedIpv4Server;

private:
    Utils() = default;
    ~Utils() = default;

};


#endif //BACHELOR_NS3_SIMULATOR_UTILS_HPP
