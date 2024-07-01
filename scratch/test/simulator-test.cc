/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "simulator-test.h"

#include <iostream>
#include <iomanip>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ScratchSimulator"); // enable logging on terminal

int main(int argc, char* argv[])
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simulator parameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Scenario parameters (that we will use inside this script):
    uint16_t interUeDistance = INTER_UE_DISTANCE; // meters
    //bool logging = false;

    // Traffic parameters (that we will use inside this script:)
    //bool useIPv6 = false; // default IPV4
    uint32_t udpPacketSizeBe = UPD_PACKET_SIZE;
    double dataRateBe = DATA_RATE_BE; // 16 kilobits per second

    // Simulation parameters.
    Time simTime = Seconds(SIMULATION_TIME);
    // Sidelink bearers activation time
    Time slBearersActivationTime = Seconds(SIMULATION_SL_BEARER_ACTIVATION_TIME);

    // NR parameters. We will take the input from the command line, and then we
    // will pass them inside the NR module.
    double centralFrequencyBandSl = CENTRAL_FREQUENCY_BAND_SL; // band n47  TDD //Here band is analogous to channel
    uint16_t bandwidthBandSl = BANDWIDTH_BAND_SL;         // Multiple of 100 KHz; 400 = 40 MHz

    // Where we will store the output files.
    std::string simTag = "default";
    std::string outputDir = "./outputs";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simulator configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    NS_LOG_UNCOND("Scratch Simulator");
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);

    //allows to see the message of a specific component in the terminal
    //LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    //LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    Config::SetDefault("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue(999999999));

    // Final simulation time is the addition of:
    // simTime + slBearersActivationTime + 10 ms additional delay for UEs to activate the bearers
    Time finalSlBearersActivationTime = slBearersActivationTime + Seconds(0.01);
    Time finalSimTime = simTime + finalSlBearersActivationTime;
    std::cout << "Final Simulation duration " << finalSimTime.GetSeconds() << std::endl;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Topology configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //User Equipment
    NodeContainer ueVoiceContainer;
    ueVoiceContainer.Create(NUM_UE);

    // Static position of the UEs
    MobilityHelper mobility;
    mobility.SetMobilityModel(MOBILITY_MODEL);
    Ptr<ListPositionAllocator> positionAllocUe = CreateObject<ListPositionAllocator>();
    for (uint16_t i = 0; i < NUM_UE; i++)
    {
        positionAllocUe->Add(Vector(interUeDistance * i, 0.0, 1.5));
    }
    mobility.SetPositionAllocator(positionAllocUe);
    mobility.Install(ueVoiceContainer);

    // Topology
    //           UE1..........(dist)..........UE2
    //    (0.0, 0.0, 1.5)               (20, 0.0, 1.5)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NR configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Helpers
    Ptr<NrPointToPointEpcHelper> epcHelper = CreateObject<NrPointToPointEpcHelper>(); //core network
    Ptr<NrHelper> nrHelper = CreateObject<NrHelper>(); // NR stack
    nrHelper->SetEpcHelper(epcHelper);

    // Create the bands
    BandwidthPartInfoPtrVector allBwps;
    CcBwpCreator ccBwpCreator;

    BandwidthPartInfo::Scenario scenario = BandwidthPartInfo::Scenario::UMi_StreetCanyon;

    CcBwpCreator::SimpleOperationBandConf bandConfSl(centralFrequencyBandSl,
                                                     bandwidthBandSl,
                                                     NUM_CC_PER_BAND,
                                                         scenario);

    // Create the bands
    OperationBandInfo bandSl = ccBwpCreator.CreateOperationBandContiguousCc(bandConfSl);

    /////////////////////////////////////////
    // The configured spectrum division is:
    // ------------Band1--------------
    // ------------CC1----------------
    // ------------BwpSl--------------
    /////////////////////////////////////////

    // Maybe I can't do what I want with these parameters :
    Config::SetDefault("ns3::ThreeGppChannelModel::UpdatePeriod", TimeValue(MilliSeconds(THREE_GPP_CHANNEL_MODEL_UPDATE_PERIOD)));
    nrHelper->SetChannelConditionModelAttribute("UpdatePeriod", TimeValue(MilliSeconds(NR_H_CHANNEL_MODEL_UPDATE_PERIOD_CONDITION)));
    nrHelper->SetPathlossAttribute("ShadowingEnabled", BooleanValue(NR_H_SHADOWING_ENABLED));

    // initialize all the spectrum
    nrHelper->InitializeOperationBand(&bandSl);
    allBwps = CcBwpCreator::GetAllBwps({bandSl});

    // setup the nodes with the NR stack
    Packet::EnableChecking();
    Packet::EnablePrinting();

    ///////////////////////////////////////////////////////////////////
    // 1. Configure the attribute through the helper, and then install;
    // Core latency
    epcHelper->SetAttribute("S1uLinkDelay", TimeValue(MilliSeconds(EPC_H_S1uLinkDelay)));

    //antenna configuration quasi-omnidirectional transmission and reception (default configuration of the beams)
    nrHelper->SetUeAntennaAttribute("NumRows", UintegerValue(NR_H_ANTENNA_NumRows));
    nrHelper->SetUeAntennaAttribute("NumColumns", UintegerValue(NR_H_ANTENNA_NumColumns));
    nrHelper->SetUeAntennaAttribute("AntennaElement",PointerValue(CreateObject<IsotropicAntennaModel>()));

    // Physical layer configuration
    nrHelper->SetUePhyAttribute("TxPower", DoubleValue(NR_H_PHY_TxPower));

    // Mac layer configuration
    nrHelper->SetUeMacAttribute("EnableSensing", BooleanValue(NR_H_MAC_EnableSensing));
    nrHelper->SetUeMacAttribute("T1", UintegerValue(NR_H_MAC_T1));
    nrHelper->SetUeMacAttribute("T2", UintegerValue(NR_H_MAC_T2));
    nrHelper->SetUeMacAttribute("ActivePoolId", UintegerValue(NR_H_MAC_ActivePoolId));
    nrHelper->SetUeMacAttribute("ReservationPeriod", TimeValue(MilliSeconds(NR_H_MAC_ReservationPeriod)));
    nrHelper->SetUeMacAttribute("NumSidelinkProcess", UintegerValue(NR_H_MAC_NumSidelinkProcess));
    nrHelper->SetUeMacAttribute("EnableBlindReTx", BooleanValue(NR_H_MAC_EnableBlindReTx));


    nrHelper->SetBwpManagerTypeId(TypeId::LookupByName(NR_H_MANAGER_TYPE));
    // following parameter has no impact at the moment because:
    // 1. No support for PQI based mapping between the application and the LCs
    // 2. No scheduler to consider PQI
    // However, till such time all the NR SL examples should use GBR_MC_PUSH_TO_TALK
    // because we hard coded the PQI 65 in UE RRC.
    nrHelper->SetUeBwpManagerAlgorithmAttribute("GBR_MC_PUSH_TO_TALK",
                                                UintegerValue(NR_H_MAC_MANAGER_GBR_MC_PUSH_TO_TALK));

    std::set<uint8_t> bwpIdContainer;
    bwpIdContainer.insert(NR_H_MAC_MANAGER_GBR_MC_PUSH_TO_TALK);

    ////////////////////////////////////////////////////////////
    // 2. Attributes valid for a subset of the nodes
    // NOT PRESENT IN THIS SIMPLE EXAMPLE

    /*
     * We have configured the attributes we needed. Now, install and get the pointers
     * to the NetDevices, which contains all the NR stack:
     */
    NetDeviceContainer ueVoiceNetDev = nrHelper->InstallUeDevice(ueVoiceContainer, allBwps);


    ////////////////////////////////////////////////////////////////////////
    // 3. Go node for node and change the attributes we have to set up per-node.
    // NOT PRESENT IN THIS SIMPLE EXAMPLE


    // update all configuration for the UE devices
    for (auto it = ueVoiceNetDev.Begin(); it != ueVoiceNetDev.End(); ++it)
    {
        DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();
    }

    // Sidelink configuration
    Ptr<NrSlHelper> nrSlHelper = CreateObject<NrSlHelper>();
    nrSlHelper->SetEpcHelper(epcHelper);

    // Error model and the adaptive modulation coding with the MCS (AMC)
    nrSlHelper->SetSlErrorModel(SL_H_ERROR_MODEL);
    // AMC -> Adaptative Modulation and Coding
    nrSlHelper->SetUeSlAmcAttribute("AmcModel", EnumValue(NrAmc::ErrorModel));

    // Sidelink scheduler attributes with fix MCS value
    nrSlHelper->SetNrSlSchedulerTypeId(NrSlUeMacSchedulerSimple::GetTypeId());
    nrSlHelper->SetUeSlSchedulerAttribute("FixNrSlMcs", BooleanValue(SL_H_FixNrSlMcs));
    nrSlHelper->SetUeSlSchedulerAttribute("InitialNrSlMcs", UintegerValue(SL_H_InitialNrSlMcs));

    // IMPORTANT: Prepare the UEs for sidelink
    nrSlHelper->PrepareUeForSidelink(ueVoiceNetDev, bwpIdContainer);

    // Resource pool configuration
    LteRrcSap::SlResourcePoolNr slResourcePoolNr;
    Ptr<NrSlCommPreconfigResourcePoolFactory> ptrFactory = Create<NrSlCommPreconfigResourcePoolFactory>();

    std::vector<std::bitset<1>> slBitmap = {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1};

    ptrFactory->SetSlTimeResources(slBitmap);
    ptrFactory->SetSlSensingWindow(F_SL_TIME_RESOURCE); // T0 in ms
    ptrFactory->SetSlSelectionWindow(F_SL_SELECTION_WINDOW);
    ptrFactory->SetSlFreqResourcePscch(F_SL_FREQ_RESOURCE_PSCCH); // PSCCH RBs
    ptrFactory->SetSlSubchannelSize(F_SL_SUBCHANNEL_SIZE);
    ptrFactory->SetSlMaxNumPerReserve(F_SL_MAX_NUM_PER_RESERVE);
    // Once parameters are configured, we can create the pool
    LteRrcSap::SlResourcePoolNr pool = ptrFactory->CreatePool();
    slResourcePoolNr = pool;

    uint16_t poolId = 0;
    LteRrcSap::SlResourcePoolIdNr slResourcePoolIdNr;
    slResourcePoolIdNr.id = poolId;

    LteRrcSap::SlResourcePoolConfigNr slresoPoolConfigNr;
    slresoPoolConfigNr.haveSlResourcePoolConfigNr = SL_RESOURCE_POOL_CONFIG_NR;


    slresoPoolConfigNr.slResourcePoolId = slResourcePoolIdNr;
    slresoPoolConfigNr.slResourcePool = slResourcePoolNr;

    LteRrcSap::SlBwpPoolConfigCommonNr slBwpPoolConfigCommonNr;
    slBwpPoolConfigCommonNr.slTxPoolSelectedNormal[slResourcePoolIdNr.id] = slresoPoolConfigNr;

    // Configure the BWP IE
    LteRrcSap::Bwp bwp;
    bwp.numerology = SL_NUMEROLOGY_BWP;
    bwp.symbolsPerSlots = SL_SYMBOL_PER_SLOTS;
    bwp.rbPerRbg = SL_RB_PER_RB_GROUP;
    bwp.bandwidth = bandwidthBandSl;

    // Configure the SlBwpGeneric IE
    LteRrcSap::SlBwpGeneric slBwpGeneric;
    slBwpGeneric.bwp = bwp;
    slBwpGeneric.slLengthSymbols = LteRrcSap::GetSlLengthSymbolsEnum(SL_NBR_SYMBOL_ALLOCATED_PER_SLOT);
    slBwpGeneric.slStartSymbol = LteRrcSap::GetSlStartSymbolEnum(SL_FIRST_SYMBOL_ALLOCATED_IN_SLOT);

    // Configure the SlBwpConfigCommonNr IE
    LteRrcSap::SlBwpConfigCommonNr slBwpConfigCommonNr;
    slBwpConfigCommonNr.haveSlBwpGeneric = SL_BWP_GENERIC;
    slBwpConfigCommonNr.slBwpGeneric = slBwpGeneric;
    slBwpConfigCommonNr.haveSlBwpPoolConfigCommonNr = SL_BWP_POOL_CONFIG_COMMON_NR;
    slBwpConfigCommonNr.slBwpPoolConfigCommonNr = slBwpPoolConfigCommonNr;

    LteRrcSap::SlFreqConfigCommonNr slFreConfigCommonNr;
    for (const auto& it : bwpIdContainer)
    {
        // it is the BWP id
        slFreConfigCommonNr.slBwpList[it] = slBwpConfigCommonNr;
    }

    // Configure the TddUlDlConfigCommon IE
    LteRrcSap::TddUlDlConfigCommon tddUlDlConfigCommon;
    tddUlDlConfigCommon.tddPattern = SL_TDD_PATTERN;

    // Configure the SlPreconfigGeneralNr IE
    LteRrcSap::SlPreconfigGeneralNr slPreconfigGeneralNr;
    slPreconfigGeneralNr.slTddConfig = tddUlDlConfigCommon;

    // Configure the SlUeSelectedConfig IE
    LteRrcSap::SlUeSelectedConfig slUeSelectedPreConfig;
    slUeSelectedPreConfig.slProbResourceKeep = SL_PROB_RESOURCE_KEEP;
    // Configure the SlPsschTxParameters IE
    LteRrcSap::SlPsschTxParameters psschParams;
    psschParams.slMaxTxTransNumPssch = SL_MAX_TX_TRANS_NUM_PSSCH;
    // Configure the SlPsschTxConfigList IE
    LteRrcSap::SlPsschTxConfigList pscchTxConfigList;
    pscchTxConfigList.slPsschTxParameters[0] = psschParams;
    slUeSelectedPreConfig.slPsschTxConfigList = pscchTxConfigList;

    LteRrcSap::SidelinkPreconfigNr slPreConfigNr;
    slPreConfigNr.slPreconfigGeneral = slPreconfigGeneralNr;
    slPreConfigNr.slUeSelectedPreConfig = slUeSelectedPreConfig;
    slPreConfigNr.slPreconfigFreqInfoList[0] = slFreConfigCommonNr;

    // Communicate the above pre-configuration to the NrSlHelper
    nrSlHelper->InstallNrSlPreConfiguration(ueVoiceNetDev, slPreConfigNr);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IP configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Stream configuration
    int64_t stream = 1;
    stream += nrHelper->AssignStreams(ueVoiceNetDev, stream);
    stream += nrSlHelper->AssignStreams(ueVoiceNetDev, stream);

    InternetStackHelper internet;
    internet.Install(ueVoiceContainer);
    stream += internet.AssignStreams(ueVoiceContainer, stream);
    uint32_t dstL2Id = DST_L2_ID;
    Ipv4Address groupAddress4(IP_GROUP_ADDRESS); // use multicast address as destination
    Address remoteAddress;
    Address localAddress;
    uint16_t port = PORT;
    // each node need a traffic flow template (TFT) to manage like a filter the incoming packets and
    // improve the efficiency with the ip address and the port...
    Ptr<LteSlTft> tft;

    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(ueVoiceNetDev);

    // set the default gateway for the UE --> usefully to count the packet and the bits sent and received
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    for (uint32_t u = 0; u < ueVoiceContainer.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueVoiceContainer.Get(u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }
    remoteAddress = InetSocketAddress(groupAddress4, port);
    localAddress = InetSocketAddress(Ipv4Address::GetAny(), port);
    tft = Create<LteSlTft>(LteSlTft::Direction::BIDIRECTIONAL, LteSlTft::CommType::GroupCast, groupAddress4, dstL2Id);
    // Set Sidelink bearers
    nrSlHelper->ActivateNrSlBearer(finalSlBearersActivationTime, ueVoiceNetDev, tft);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    OnOffHelper sidelinkClient("ns3::UdpSocketFactory", remoteAddress);
    sidelinkClient.SetAttribute("EnableSeqTsSizeHeader", BooleanValue(APP_EnableSeqTsSizeHeader));
    std::string dataRateBeString = std::to_string(dataRateBe) + "kb/s";
    std::cout << "Data rate " << DataRate(dataRateBeString) << std::endl;
    sidelinkClient.SetConstantRate(DataRate(dataRateBeString), udpPacketSizeBe);

    ApplicationContainer clientApps = sidelinkClient.Install(ueVoiceContainer.Get(0));
    // onoff application will send the first packet at :
    // finalSlBearersActivationTime + ((Pkt size in bits) / (Data rate in bits per sec))
    clientApps.Start(finalSlBearersActivationTime);
    clientApps.Stop(finalSimTime);

    // Output app start, stop and duration
    double realAppStart =
            finalSlBearersActivationTime.GetSeconds() +
            ((double)udpPacketSizeBe * 8.0 / (DataRate(dataRateBeString).GetBitRate()));
    double appStopTime = (finalSimTime).GetSeconds();

    std::cout << "App start time " << realAppStart << " sec" << std::endl;
    std::cout << "App stop time " << appStopTime << " sec" << std::endl;

    ApplicationContainer serverApps;
    PacketSinkHelper sidelinkSink("ns3::UdpSocketFactory", localAddress);
    sidelinkSink.SetAttribute("EnableSeqTsSizeHeader", BooleanValue(true));
    serverApps = sidelinkSink.Install(ueVoiceContainer.Get(ueVoiceContainer.GetN() - 1));
    serverApps.Start(Seconds(2.0));


    // Trace receptions; use the following to be robust to node ID changes
    std::ostringstream path;
    path << "/NodeList/" << ueVoiceContainer.Get(1)->GetId()
         << "/ApplicationList/0/$ns3::PacketSink/Rx";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::ReceivePacket));
    path.str("");

    path << "/NodeList/" << ueVoiceContainer.Get(1)->GetId()
         << "/ApplicationList/0/$ns3::PacketSink/Rx";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::ComputePir));
    path.str("");

    path << "/NodeList/" << ueVoiceContainer.Get(0)->GetId()
         << "/ApplicationList/0/$ns3::OnOffApplication/Tx";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::TransmitPacket));
    path.str("");

    // Datebase setup
    std::string exampleName = simTag + "-" + "simulator-outputs";
    SQLiteOutput db(outputDir + exampleName + ".db");

    UeToUePktTxRxOutputStats pktStats;
    pktStats.SetDb(&db, "pktTxRx");

    // Set Tx traces
    for (uint16_t ac = 0; ac < clientApps.GetN(); ac++)
    {
        Ipv4Address localAddrs = clientApps.Get(ac)
                ->GetNode()
                ->GetObject<Ipv4L3Protocol>()
                ->GetAddress(1, 0)
                .GetLocal();
        std::cout << "Tx address: " << localAddrs << std::endl;
        clientApps.Get(ac)->TraceConnect("TxWithSeqTsSize",
                                         "tx",
                                         MakeBoundCallback(&Utils::UePacketTraceDb,
                                                           &pktStats,
                                                           ueVoiceContainer.Get(0),
                                                           localAddrs));
    }

    // Set Rx traces
    for (uint16_t ac = 0; ac < serverApps.GetN(); ac++)
    {
        Ipv4Address localAddrs = serverApps.Get(ac)
                ->GetNode()
                ->GetObject<Ipv4L3Protocol>()
                ->GetAddress(1, 0)
                .GetLocal();
        std::cout << "Rx address: " << localAddrs << std::endl;
        serverApps.Get(ac)->TraceConnect("RxWithSeqTsSize",
                                         "rx",
                                         MakeBoundCallback(&Utils::UePacketTraceDb,
                                                           &pktStats,
                                                           ueVoiceContainer.Get(1),
                                                           localAddrs));
    }

    Simulator::Stop(finalSimTime);
    Simulator::Run();

    std::cout << "Total Tx bits = " << Utils::txByteCounter * 8 << std::endl;
    std::cout << "Total Tx packets = " << Utils::txPktCounter << std::endl;

    std::cout << "Total Rx bits = " << Utils::rxByteCounter * 8 << std::endl;
    std::cout << "Total Rx packets = " << Utils::rxPktCounter << std::endl;

    std::cout << "Avrg thput = "
              << (Utils::rxByteCounter * 8) / (finalSimTime - Seconds(realAppStart)).GetSeconds() / 1000.0
              << " kbps" << std::endl;

    std::cout << "Average Packet Inter-Reception (PIR) " << Utils::pir.GetSeconds() / Utils::pirCounter << " sec"
              << std::endl;

    /*
     * VERY IMPORTANT: Do not forget to empty the database cache, which would
     * dump the data store towards the end of the simulation in to a database.
     */
    pktStats.EmptyCache();

    Simulator::Destroy();

    return 0;
}
