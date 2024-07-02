//
// Created by ranium on 21.06.24.
//

#include "utils.h"

using namespace ns3;

//initialize static variables

uint32_t Utils::rxByteCounter(0);
uint32_t Utils::txByteCounter(0);
uint32_t Utils::rxPktCounter(0);
uint32_t Utils::txPktCounter(0);

uint64_t Utils::pirCounter(0);
Time Utils::lastPktRxTime(0);
Time Utils::pir(0);



void Utils::NotifySlPscchScheduling(UeMacPscchTxOutputStats* pscchStats, const SlPscchUeMacStatParameters pscchStatsParams)
{
    pscchStats->ns3::UeMacPscchTxOutputStats::Save(pscchStatsParams);

}

void Utils::NotifySlPsschScheduling(UeMacPsschTxOutputStats* psschStats, const SlPsschUeMacStatParameters psschStatsParams)
{
    psschStats->Save(psschStatsParams);
}

void Utils::NotifySlPscchRx(UePhyPscchRxOutputStats* pscchStats, const SlRxCtrlPacketTraceParams pscchStatsParams)
{
    pscchStats->Save(pscchStatsParams);
}

void Utils::NotifySlPsschRx(UePhyPsschRxOutputStats* psschStats, const SlRxDataPacketTraceParams psschStatsParams)
{
    psschStats->Save(psschStatsParams);
}

void Utils::UePacketTraceDb(UeToUePktTxRxOutputStats* stats,
                Ptr<Node> node,
                const Address& localAddrs,
                std::string txRx,
                Ptr<const Packet> p,
                const Address& srcAddrs,
                const Address& dstAddrs,
                const SeqTsSizeHeader& seqTsSizeHeader)
{
    uint32_t nodeId = node->GetId();
    uint64_t imsi = node->GetDevice(0)->GetObject<NrUeNetDevice>()->GetImsi();
    uint32_t seq = seqTsSizeHeader.GetSeq();
    uint32_t pktSize = p->GetSize() + seqTsSizeHeader.GetSerializedSize();

    stats->Save(txRx, localAddrs, nodeId, imsi, pktSize, srcAddrs, dstAddrs, seq);
}

void Utils::ReceivePacket(Ptr<const Packet> packet, const Address&)
{
    rxByteCounter += packet->GetSize();
    rxPktCounter++;
}

void Utils::TransmitPacket(Ptr<const Packet> packet)
{
    txByteCounter += packet->GetSize();
    txPktCounter++;
}

void Utils::ComputePir(Ptr<const Packet> packet, const Address&)
{
    if (pirCounter == 0 && lastPktRxTime.GetSeconds() == 0.0)
    {
        // this the first packet, just store the time and get out
        lastPktRxTime = Simulator::Now();
        return;
    }
    pir = pir + (Simulator::Now() - lastPktRxTime);
    lastPktRxTime = Simulator::Now();
    pirCounter++;
}

void Utils::packetTx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
    std::cout << "Packet Tx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
}

void Utils::packetRx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
    std::cout << "Packet Rx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
}

