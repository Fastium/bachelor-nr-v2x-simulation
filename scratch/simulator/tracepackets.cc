//
// Created by ranium on 26.06.24.
//

#include "tracepackets.h"

uint32_t Utils::rxByteCounter(0);
uint32_t Utils::txByteCounter(0);
uint32_t Utils::rxPktCounter(0);
uint32_t Utils::txPktCounter(0);

uint64_t Utils::pirCounter(0);
Time Utils::lastPktRxTime(0);
Time Utils::pir(0);

TracePackets::TracePackets() {

}

void TracePackets::create(UserEquipments * userEquipments) {
    std::ostringstream path;
    path << "/NodeList/" << userEquipments->getUeVoiceContainer()->Get(1)->GetId()
         << "/ApplicationList/0/$ns3::PacketSink/Rx";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&TracePackets::ReceivePacket));
    path.str("");

    path << "/NodeList/" << userEquipments->getUeVoiceContainer()->Get(1)->GetId()
         << "/ApplicationList/0/$ns3::PacketSink/Rx";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&TracePackets::ComputePir));
    path.str("");

    path << "/NodeList/" << userEquipments->getUeVoiceContainer()->Get(0)->GetId()
         << "/ApplicationList/0/$ns3::OnOffApplication/Tx";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&TracePackets::TransmitPacket));
    path.str("");
}

void TracePackets::NotifySlPscchScheduling(UeMacPscchTxOutputStats* pscchStats, const SlPscchUeMacStatParameters pscchStatsParams)
{
    pscchStats->ns3::UeMacPscchTxOutputStats::Save(pscchStatsParams);

}

void TracePackets::NotifySlPsschScheduling(UeMacPsschTxOutputStats* psschStats, const SlPsschUeMacStatParameters psschStatsParams)
{
    psschStats->Save(psschStatsParams);
}

void TracePackets::NotifySlPscchRx(UePhyPscchRxOutputStats* pscchStats, const SlRxCtrlPacketTraceParams pscchStatsParams)
{
    pscchStats->Save(pscchStatsParams);
}

void TracePackets::NotifySlPsschRx(UePhyPsschRxOutputStats* psschStats, const SlRxDataPacketTraceParams psschStatsParams)
{
    psschStats->Save(psschStatsParams);
}

void TracePackets::UePacketTraceDb(UeToUePktTxRxOutputStats* stats,
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

void TracePackets::ReceivePacket(Ptr<const Packet> packet, const Address&)
{
    rxByteCounter += packet->GetSize();
    rxPktCounter++;
}

void TracePackets::TransmitPacket(Ptr<const Packet> packet)
{
    txByteCounter += packet->GetSize();
    txPktCounter++;
}

void TracePackets::ComputePir(Ptr<const Packet> packet, const Address&)
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

void TracePackets::emptyCache() {
    pktStats.EmptyCache();
    pscchStats.EmptyCache();
    psschStats.EmptyCache();
    pscchPhyStats.EmptyCache();
    psschPhyStats.EmptyCache();
}

void TracePackets::setupDatabase(TraceConfig_t * traceConfig, App * app, UserEquipments * userEquipments) {
    std::string exampleName = traceConfig->simTag + "-" + "simulator-outputs";
    db = new SQLiteOutput(traceConfig->outputDir + exampleName + ".db");

    pscchStats.SetDb(db, "pscchTxUeMac");
    Config::ConnectWithoutContext("/NodeList/*/DeviceList/*/$ns3::NrUeNetDevice/"
                                  "ComponentCarrierMapUe/*/NrUeMac/SlPscchScheduling",
                                  MakeBoundCallback(&TracePackets::NotifySlPscchScheduling, &pscchStats));

    psschStats.SetDb(db, "psschTxUeMac");
    Config::ConnectWithoutContext("/NodeList/*/DeviceList/*/$ns3::NrUeNetDevice/"
                                  "ComponentCarrierMapUe/*/NrUeMac/SlPsschScheduling",
                                  MakeBoundCallback(&TracePackets::NotifySlPsschScheduling, &psschStats));

    pscchPhyStats.SetDb(db, "pscchRxUePhy");
    Config::ConnectWithoutContext(
            "/NodeList/*/DeviceList/*/$ns3::NrUeNetDevice/ComponentCarrierMapUe/*/NrUePhy/"
            "NrSpectrumPhyList/*/RxPscchTraceUe",
            MakeBoundCallback(&TracePackets::NotifySlPscchRx, &pscchPhyStats));

    psschPhyStats.SetDb(db, "psschRxUePhy");
    Config::ConnectWithoutContext(
            "/NodeList/*/DeviceList/*/$ns3::NrUeNetDevice/ComponentCarrierMapUe/*/NrUePhy/"
            "NrSpectrumPhyList/*/RxPsschTraceUe",
            MakeBoundCallback(&TracePackets::NotifySlPsschRx, &psschPhyStats));

    pktStats.SetDb(db, "pktTxRx");

    // Set Tx traces
    for (uint16_t ac = 0; ac < app->getClientApps()->GetN(); ac++)
    {
        Ipv4Address localAddrs = app->getClientApps()->Get(ac)
                ->GetNode()
                ->GetObject<Ipv4L3Protocol>()
                ->GetAddress(1, 0)
                .GetLocal();
        std::cout << "Tx address: " << localAddrs << std::endl;
        app->getClientApps()->Get(ac)->TraceConnect("TxWithSeqTsSize",
                                         "tx",
                                         MakeBoundCallback(&Utils::UePacketTraceDb,
                                                           &pktStats,
                                                           userEquipments->getUeVoiceContainer()->Get(0),
                                                           localAddrs));
    }

    // Set Rx traces
    for (uint16_t ac = 0; ac < app->getServerApps()->GetN(); ac++)
    {
        Ipv4Address localAddrs = app->getServerApps()->Get(ac)
                ->GetNode()
                ->GetObject<Ipv4L3Protocol>()
                ->GetAddress(1, 0)
                .GetLocal();
        std::cout << "Rx address: " << localAddrs << std::endl;
        app->getServerApps()->Get(ac)->TraceConnect("RxWithSeqTsSize",
                                         "rx",
                                         MakeBoundCallback(&Utils::UePacketTraceDb,
                                                           &pktStats,
                                                           userEquipments->getUeVoiceContainer()->Get(1),
                                                           localAddrs));
    }

}
