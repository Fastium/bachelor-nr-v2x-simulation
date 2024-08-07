//
// Created by ranium on 08.07.24.
//

#include "configuration.h"

#include <iomanip>

using namespace ns3;

void frameRx(PacketOutputDb* db, const SlRxDataPacketTraceParams traceParams)
{
    db->SavePacketRxPhySpectrum(traceParams);
}

void frameTx(PacketOutputDb* db, const SlTxDataPacketTraceParams traceParams)
{
    db->SavePacketTxPhySpectrum(traceParams);
}

NS_LOG_COMPONENT_DEFINE("V2X-simulator"); // enable logging on terminal

int main(int argc, char* argv[])
{
    NS_LOG_UNCOND("Scratch Simulator");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project parameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Application parameters
    uint32_t udpPacketSizeBe = UPD_PACKET_SIZE;
    double dataRateBe = DATA_RATE_BE;
    double simulationTime = SIMULATION_TIME;
    double bearerActivationTime = BEARER_ACTIVATION_TIME;
    double bearerDelay = BEARER_ACTIVATION_DELAY;
    double serverStartTime = SERVER_START_TIME;

    // Physical parameters
    uint16_t numerologyBwpSl = SL_NUMEROLOGY_BWP;
    uint16_t subchannelSize = SL_SUBCHANNEL_SIZE;
    double txPower = NR_H_PHY_TxPower;
    uint32_t phyNoise = PHY_NOISE;
    uint32_t phyLatency = PHY_LATENCY;
    double centralFrequencyBandSl = CENTRAL_FREQUENCY_BAND_SL; // band n47  TDD //Here band is analogous to channel
    uint16_t bandwidthBandSl = BANDWIDTH_BAND_SL;         // Multiple of 100 KHz; 400 = 40 MHz
    uint32_t antennaNumRows = ANTENNA_NumRows;
    uint32_t antennaNumColumns = ANTENNA_NumColumns;

    // MAC parameters
    bool enableSensing = EnableSensing;
    uint32_t t1 = MAC_T1;
    uint32_t t2 = MAC_T2;
    uint32_t activePoolId = MAC_ActivePoolId;
    double reservationPeriod = MAC_ReservationPeriod;
    uint32_t numSidelinkProcess = MAC_NumSidelinkProcess;
    uint32_t initialMcs = 28;

    // Node parameters
    uint32_t numRouters = NUM_ROUTERS;
    double ueDistance = UE_DISTANCE;

    // Global parameters
    std::string simTag = SIM_TAG;
    std::string errorModel = ERROR_MODEL;
    std::string scenario = SCENARIO;
    double sidelinkDelay = SIDELINK_DELAY;
    bool displayParameter = false;


    CommandLine cmd(__FILE__);
    cmd.Usage ("...");
    cmd.AddValue("udpPacketSizeBe", "UDP packet size for best effort traffic", udpPacketSizeBe);
    cmd.AddValue("dataRateBe", "Data rate for best effort traffic", dataRateBe);
    cmd.AddValue("simulationTime", "Simulation time", simulationTime);
    cmd.AddValue("numerologyBwpSl", "Numerology of the SL BWP", numerologyBwpSl);
    cmd.AddValue("txPower", "Transmission power of the UEs", txPower);
    cmd.AddValue("phyNoise", "Noise power", phyNoise);
    cmd.AddValue("phyLatency", "Latency of the PHY layer", phyLatency);
    cmd.AddValue("centralFrequencyBandSl", "Central frequency of the SL BWP", centralFrequencyBandSl);
    cmd.AddValue("bandwidthBandSl", "Bandwidth of the SL BWP", bandwidthBandSl);
    cmd.AddValue("antennaNumRows", "Number of rows of the antenna", antennaNumRows);
    cmd.AddValue("antennaNumColumns", "Number of columns of the colomns", antennaNumColumns);
    cmd.AddValue("enableSensing", "Enable the sensing", enableSensing);
    cmd.AddValue("t1", "T1", t1);
    cmd.AddValue("t2", "T2", t2);
    cmd.AddValue("activePoolId", "Active pool ID", activePoolId);
    cmd.AddValue("reservationPeriod", "Reservation period", reservationPeriod);
    cmd.AddValue("numSidelinkProcess", "Number of sidelink process", numSidelinkProcess);
    cmd.AddValue("numRouters", "Number of router", numRouters);
    cmd.AddValue("ueDistance", "Distance between SRC and DST", ueDistance);
    cmd.AddValue("simTag", "Simulation tag", simTag);
    cmd.AddValue("errorModel", "Error model", errorModel);
    cmd.AddValue("scenario", "Scenario", scenario);
    cmd.AddValue("sidelinkDelay", "Delay of the S1u link in milliseconds", sidelinkDelay);
    cmd.AddValue("serverStartTime", "Server start time", serverStartTime);
    cmd.AddValue("bearerActivationTime", "Bearer activation time", bearerActivationTime);
    cmd.AddValue("bearerDelay", "Bearer delay", bearerDelay);
    cmd.AddValue("initialMcs", "Initial MCS", initialMcs);
    cmd.Parse(argc, argv);

    std::cout << std::endl;

    if(displayParameter == true)
    {
        std::cout << std::endl;
        std::cout << "Simulation parameters : " << std::endl;
        std::cout << "UDP packet size for best effort traffic : " << udpPacketSizeBe << std::endl;
        std::cout << "Data rate for best effort traffic : " << dataRateBe << std::endl;
        std::cout << "Simulation time : " << simulationTime << std::endl;
        std::cout << "Numerology of the SL BWP : " << numerologyBwpSl << std::endl;
        std::cout << "Transmission power of the UEs : " << txPower << std::endl;
        std::cout << "Noise power : " << phyNoise << std::endl;
        std::cout << "Latency of the PHY layer : " << phyLatency << std::endl;
        std::cout << "Central frequency of the SL BWP : " << centralFrequencyBandSl << std::endl;
        std::cout << "Bandwidth of the SL BWP : " << bandwidthBandSl << std::endl;
        std::cout << "Number of rows of the antenna : " << antennaNumRows << std::endl;
        std::cout << "Number of columns of the colomns : " << antennaNumColumns << std::endl;
        std::cout << "Enable the sensing : " << enableSensing << std::endl;
        std::cout << "T1 : " << t1 << std::endl;
        std::cout << "T2 : " << t2 << std::endl;
        std::cout << "Active pool ID : " << activePoolId << std::endl;
        std::cout << "Reservation period : " << reservationPeriod << std::endl;
        std::cout << "Number of sidelink process : " << numSidelinkProcess << std::endl;
        std::cout << "Number of router : " << numRouters << std::endl;
        std::cout << "Distance between user equipment: " << ueDistance << std::endl;
        std::cout << "Simulation tag : " << simTag << std::endl;
        std::cout << "Error model : " << errorModel << std::endl;
        std::cout << "Scenario : " << scenario << std::endl;
        std::cout << "Delay of the S1u link in milliseconds : " << sidelinkDelay << std::endl;
        std::cout << "Server start time : " << serverStartTime << std::endl;
        std::cout << "Bearer activation time : " << bearerActivationTime << std::endl;
        std::cout << "Bearer delay : " << bearerDelay << std::endl;
        std::cout << "Initial MCS : " << initialMcs << std::endl;
        std::cout << std::endl << std::endl;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project parameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    LogComponentEnable("ScratchSimulator", LOG_LEVEL_ALL);
//    LogComponentEnable("NrSlHelper", LOG_LEVEL_ALL);
//    LogComponentEnable("LteUeRrc", LOG_LEVEL_ALL);
//    LogComponentEnable("NrNetDevice", LOG_LEVEL_ALL);
//    LogComponentEnable("Ipv4L3Protocol", LOG_LEVEL_LOGIC);
//    LogComponentEnable("NrUeMac", LOG_LEVEL_INFO);
//    LogComponentEnable("NrSpectrumPhy", LOG_LEVEL_DEBUG);
//    LogComponentEnable("Config", LOG_LEVEL_DEBUG);
//    LogComponentEnable("NrUePhy", LOG_LEVEL_DEBUG);


    // Where we will store the output files.
    std::string outputDir = "./";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simulator parameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Scenario parameters (that we will use inside this script):
    uint32_t numUes = numRouters + 2; // Number of UEs
    uint32_t numOfNetworks = numUes - 1;

    // TIMING
    // Simulation parameters.
    Time t_simulation = Seconds(simulationTime);
    // Sidelink bearers activation time
    Time t_slBearersActivation = Seconds(bearerActivationTime);
    Time t_slBearersDelay = Seconds(bearerDelay);

    Time t_finalActivationBearers = t_slBearersActivation + t_slBearersDelay;
    Time t_finalSimulation = t_finalActivationBearers + t_simulation;
    Time t_serverStart = Seconds(serverStartTime);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simulator configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Time::SetResolution(Time::NS);
    Config::SetDefault("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue(999999999));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Topology configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Ptr<Node> src = CreateObject<Node>();
    NodeContainer routers;
    if(numRouters > 0)
    {
        for(uint32_t i = 0; i < numRouters; i++)
        {
            routers.Add(CreateObject<Node>());
        }
    }
    Ptr<Node> dst = CreateObject<Node>();

    NodeContainer allUes;
    allUes.Add(src, routers, dst);

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    Ptr<ListPositionAllocator> positionAllocUe = CreateObject<ListPositionAllocator>();
    for(uint32_t i = 0; i < numUes; i++)
    {
        positionAllocUe->Add(Vector(ueDistance * i, 0, 20));
    }

    mobility.SetPositionAllocator(positionAllocUe);
    mobility.Install(allUes);

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
    const uint8_t numCcPerBand = 1; // Number of component carriers per band

    /*
     * RMa : Returns Merchandise Authorization
     * UMa : Unlicensed Mobile Access
     * Umi : Urban Microcell
     */

    BandwidthPartInfo::Scenario bandwidthPartScenario = BandwidthPartInfo::V2V_Urban;
    if(scenario == "v2v_Highway")
    {
        bandwidthPartScenario= BandwidthPartInfo::Scenario::V2V_Highway;
    }
    else if(scenario == "v2v_Urban")
    {
        bandwidthPartScenario= BandwidthPartInfo::Scenario::V2V_Urban;
    }
    else
    {
        NS_LOG_ERROR("Scenario not supported");
    }

    CcBwpCreator::SimpleOperationBandConf bandConfSl(centralFrequencyBandSl,
                                                    bandwidthBandSl,
                                                    numCcPerBand,
                                                    bandwidthPartScenario);

    // Create the bands
    OperationBandInfo bandSl = ccBwpCreator.CreateOperationBandContiguousCc(bandConfSl);

    /////////////////////////////////////////
    // The configured spectrum division is:
    // ------------Band1--------------
    // ------------CC1----------------
    // ------------BwpSl--------------
    /////////////////////////////////////////

    // Maybe I can't do what I want with these parameters :
    Config::SetDefault("ns3::ThreeGppChannelModel::UpdatePeriod", TimeValue(MilliSeconds(0)));
    nrHelper->SetChannelConditionModelAttribute("UpdatePeriod", TimeValue(MilliSeconds(0)));
    nrHelper->SetPathlossAttribute("ShadowingEnabled", BooleanValue(false));

    // initialize all the spectrum
    nrHelper->InitializeOperationBand(&bandSl);
    allBwps = CcBwpCreator::GetAllBwps({bandSl});

    // setup the nodes with the NR stack
    Packet::EnableChecking();
    Packet::EnablePrinting();

    ///////////////////////////////////////////////////////////////////
    // 1. Configure the attribute through the helper, and then install;
    // Core latency
    epcHelper->SetAttribute("S1uLinkDelay", TimeValue(MilliSeconds(sidelinkDelay)));

    //antenna configuration quasi-omnidirectional transmission and reception (default configuration of the beams)
    nrHelper->SetUeAntennaAttribute("NumRows", UintegerValue(antennaNumRows));
    nrHelper->SetUeAntennaAttribute("NumColumns", UintegerValue(antennaNumColumns));
    nrHelper->SetUeAntennaAttribute("AntennaElement",PointerValue(CreateObject<IsotropicAntennaModel>()));

    // Physical layer configuration
    nrHelper->SetUePhyAttribute("TxPower", DoubleValue(txPower));

    // Mac layer configuration
    nrHelper->SetUeMacAttribute("EnableSensing", BooleanValue(enableSensing));
    nrHelper->SetUeMacAttribute("T1", UintegerValue(t1));
    nrHelper->SetUeMacAttribute("T2", UintegerValue(t2));
    nrHelper->SetUeMacAttribute("ActivePoolId", UintegerValue(activePoolId));
    nrHelper->SetUeMacAttribute("ReservationPeriod", TimeValue(MilliSeconds(reservationPeriod)));
    nrHelper->SetUeMacAttribute("NumSidelinkProcess", UintegerValue(numSidelinkProcess));
    nrHelper->SetUeMacAttribute("EnableBlindReTx", BooleanValue(true));

    uint8_t bwpIdForGbr = 0;

    nrHelper->SetBwpManagerTypeId(TypeId::LookupByName("ns3::NrSlBwpManagerUe"));
    // following parameter has no impact at the moment because:
    // 1. No support for PQI based mapping between the application and the LCs
    // 2. No scheduler to consider PQI
    // However, till such time all the NR SL examples should use GBR_MC_PUSH_TO_TALK
    // because we hard coded the PQI 65 in UE RRC.
    nrHelper->SetUeBwpManagerAlgorithmAttribute("GBR_MC_PUSH_TO_TALK",UintegerValue(bwpIdForGbr));


    std::set<uint8_t> bwpIdContainer;
    bwpIdContainer.insert(bwpIdForGbr);

    ////////////////////////////////////////////////////////////
    // 2. Attributes valid for a subset of the nodes
    // NOT PRESENT IN THIS SIMPLE EXAMPLE

    /*
    * We have configured the attributes we needed. Now, install and get the pointers
    * to the NetDevices, which contains all the NR stack:
    */
    NetDeviceContainer allNetDevices = nrHelper->InstallUeDevice(allUes, allBwps);


    std::vector<NetDeviceContainer> networks;

    for(uint32_t i = 0; i < numOfNetworks; i++)
    {
        NetDeviceContainer network;
        network.Add(allNetDevices.Get(i));
        network.Add(allNetDevices.Get(i+1));
        networks.push_back(network);
    }

    ////////////////////////////////////////////////////////////////////////
    // 3. Go node for node and change the attributes we have to set up per-node.
    // NOT PRESENT IN THIS SIMPLE EXAMPLE


    // update all configuration for the UE devices
    for(auto it = allNetDevices.Begin(); it != allNetDevices.End(); ++it)
    {
        DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();
    }

    // Sidelink configuration
    Ptr<NrSlHelper> nrSlHelper = CreateObject<NrSlHelper>();
    nrSlHelper->SetEpcHelper(epcHelper);

    /*
     * Set the SL error model and AMC
     * Error model type: ns3::NrEesmCcT1, ns3::NrEesmCcT2, ns3::NrEesmIrT1,
     *                   ns3::NrEesmIrT2, ns3::NrLteMiErrorModel
     * AMC type: NrAmc::ShannonModel or NrAmc::ErrorModel
     */

    // Error model and the adaptive modulation coding with the MCS (AMC)
    nrSlHelper->SetSlErrorModel(errorModel);

    // AMC -> Adaptative Modulation and Coding
    nrSlHelper->SetUeSlAmcAttribute("AmcModel", EnumValue(NrAmc::AmcModel::ErrorModel));

    // Sidelink scheduler attributes with fix MCS value
    nrSlHelper->SetNrSlSchedulerTypeId(NrSlUeMacSchedulerSimple::GetTypeId());
    nrSlHelper->SetUeSlSchedulerAttribute("FixNrSlMcs", BooleanValue(true));
    nrSlHelper->SetUeSlSchedulerAttribute("InitialNrSlMcs", UintegerValue(initialMcs));

    // IMPORTANT: Prepare the UEs for sidelink
    nrSlHelper->PrepareUeForSidelink(allNetDevices, bwpIdContainer);

    // Resource pool configuration
    LteRrcSap::SlResourcePoolNr slResourcePoolNr;
    Ptr<NrSlCommPreconfigResourcePoolFactory> ptrFactory = Create<NrSlCommPreconfigResourcePoolFactory>();

    std::vector<std::bitset<1>> slBitmap = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    ptrFactory->SetSlTimeResources(slBitmap);
    ptrFactory->SetSlSensingWindow(100); // T0 in ms
    ptrFactory->SetSlSelectionWindow(5);
    ptrFactory->SetSlFreqResourcePscch(10); // PSCCH RBs
    ptrFactory->SetSlSubchannelSize(subchannelSize);
    ptrFactory->SetSlMaxNumPerReserve(3);
    // Once parameters are configured, we can create the pool
    LteRrcSap::SlResourcePoolNr pool = ptrFactory->CreatePool();
    slResourcePoolNr = pool;

    LteRrcSap::SlResourcePoolConfigNr slresoPoolConfigNr;
    slresoPoolConfigNr.haveSlResourcePoolConfigNr = true;

    uint16_t poolId = 0;
    LteRrcSap::SlResourcePoolIdNr slResourcePoolIdNr;
    slResourcePoolIdNr.id = poolId;
    slresoPoolConfigNr.slResourcePoolId = slResourcePoolIdNr;
    slresoPoolConfigNr.slResourcePool = slResourcePoolNr;

    LteRrcSap::SlBwpPoolConfigCommonNr slBwpPoolConfigCommonNr;
    slBwpPoolConfigCommonNr.slTxPoolSelectedNormal[slResourcePoolIdNr.id] = slresoPoolConfigNr;

    // Configure the BWP IE
    LteRrcSap::Bwp bwp;
    bwp.numerology = numerologyBwpSl;
    bwp.symbolsPerSlots = 14;
    bwp.rbPerRbg = 1; // not use
    bwp.bandwidth = bandwidthBandSl;

    // Configure the SlBwpGeneric IE
    LteRrcSap::SlBwpGeneric slBwpGeneric;
    slBwpGeneric.bwp = bwp;
    slBwpGeneric.slLengthSymbols = LteRrcSap::GetSlLengthSymbolsEnum(14);
    slBwpGeneric.slStartSymbol = LteRrcSap::GetSlStartSymbolEnum(0);

    // Configure the SlBwpConfigCommonNr IE
    LteRrcSap::SlBwpConfigCommonNr slBwpConfigCommonNr;
    slBwpConfigCommonNr.haveSlBwpGeneric = true;
    slBwpConfigCommonNr.slBwpGeneric = slBwpGeneric;
    slBwpConfigCommonNr.haveSlBwpPoolConfigCommonNr = true;
    slBwpConfigCommonNr.slBwpPoolConfigCommonNr = slBwpPoolConfigCommonNr;

    LteRrcSap::SlFreqConfigCommonNr slFreConfigCommonNr;
    for (const auto& it : bwpIdContainer)
    {
       // it is the BWP id
       slFreConfigCommonNr.slBwpList[it] = slBwpConfigCommonNr;
    }

    // Configure the TddUlDlConfigCommon IE
    LteRrcSap::TddUlDlConfigCommon tddUlDlConfigCommon;
    tddUlDlConfigCommon.tddPattern = "UL|UL|UL|UL|UL|UL|UL|UL|UL|UL|";
//    tddUlDlConfigCommon.tddPattern = "DL|DL|DL|F|UL|UL|UL|UL|UL|UL|";

    // Configure the SlPreconfigGeneralNr IE
    LteRrcSap::SlPreconfigGeneralNr slPreconfigGeneralNr;
    slPreconfigGeneralNr.slTddConfig = tddUlDlConfigCommon;

    // Configure the SlUeSelectedConfig IE
    LteRrcSap::SlUeSelectedConfig slUeSelectedPreConfig;
    slUeSelectedPreConfig.slProbResourceKeep = 0;
    // Configure the SlPsschTxParameters IE
    LteRrcSap::SlPsschTxParameters psschParams;
    psschParams.slMaxTxTransNumPssch = 1; // normally 5
    // Configure the SlPsschTxConfigList IE
    LteRrcSap::SlPsschTxConfigList pscchTxConfigList;
    pscchTxConfigList.slPsschTxParameters[0] = psschParams;
    slUeSelectedPreConfig.slPsschTxConfigList = pscchTxConfigList;

    LteRrcSap::SidelinkPreconfigNr slPreConfigNr;
    slPreConfigNr.slPreconfigGeneral = slPreconfigGeneralNr;
    slPreConfigNr.slUeSelectedPreConfig = slUeSelectedPreConfig;
    slPreConfigNr.slPreconfigFreqInfoList[0] = slFreConfigCommonNr;

    // Communicate the above pre-configuration to the NrSlHelper
    nrSlHelper->InstallNrSlPreConfiguration(allNetDevices, slPreConfigNr);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IP configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Stream configuration to randomize the models
    int64_t stream = 1;
    stream += nrHelper->AssignStreams(allNetDevices, stream);
    stream += nrSlHelper->AssignStreams(allNetDevices, stream);

    InternetStackHelper internetNodes;
    internetNodes.SetIpv6StackInstall(false);
    internetNodes.Install(allUes);
    stream += internetNodes.AssignStreams(allUes, stream);

    // Assign addresses.
    Ipv4AddressHelper ipv4;

    for(uint32_t i = 0; i < numOfNetworks; i++)
    {
        char addr[20];
        sprintf(addr, "10.0.%d.0", i);
        ipv4.SetBase(Ipv4Address(addr), Ipv4Mask("255.255.255.0"));
        ipv4.Assign(networks.at(i));
    }


    uint16_t port = PORT;
    uint32_t dstL2 = DST_L2_ID;
    Ptr<LteSlTft> tft;

    Ptr<Ipv4StaticRouting> staticRouting;

    staticRouting = Ipv4RoutingHelper::GetRouting<Ipv4StaticRouting>(src->GetObject<Ipv4>()->GetRoutingProtocol());
    staticRouting->SetDefaultRoute("10.0.0.2", 1);

    if(numRouters > 0)
    {
        for (uint32_t i = 0; i < (uint32_t)numRouters - 1; i++)
        {
            char addr[20];
            sprintf(addr, "10.0.%d.2", i);
            staticRouting = Ipv4RoutingHelper::GetRouting<Ipv4StaticRouting>(
                routers.Get(i)->GetObject<Ipv4>()->GetRoutingProtocol());
            staticRouting->SetDefaultRoute(addr, 1);
        }
    }

    NS_LOG_INFO("Src      ("      <<  src->GetId() << "): " << src->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl);
    if(numRouters > 0)
    {
        for (uint32_t i = 0; i < NUM_ROUTERS; i++)
        {
            NS_LOG_INFO("Router" << i + 1 << i + 2 << " (" << routers.Get(i)->GetId() << "): "
                                 << routers.Get(i)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal()
                                 << endl);
            NS_LOG_INFO("Router" << i + 1 << i + 2 << " (" << routers.Get(i)->GetId() << "): "
                                 << routers.Get(i)->GetObject<Ipv4>()->GetAddress(1, 1).GetLocal()
                                 << endl);
        }
    }
    NS_LOG_INFO("Dst      ("      <<  dst->GetId() << "): " << dst->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl << endl);

    Ipv4Address addr_server = dst->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();
    Address socket_server = InetSocketAddress(addr_server, port);
    Address socket_client = InetSocketAddress(Ipv4Address::GetAny(), port);

    tft = Create<LteSlTft>(LteSlTft::Direction::TRANSMIT, LteSlTft::CommType::Unicast, addr_server, dstL2);
    nrSlHelper->ActivateNrSlBearer(t_finalActivationBearers, allNetDevices, tft);

    tft = Create<LteSlTft>(LteSlTft::Direction::RECEIVE, LteSlTft::CommType::Unicast, addr_server, dstL2);
    nrSlHelper->ActivateNrSlBearer(t_finalActivationBearers, allNetDevices, tft);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Set Application in the UEs
    OnOffHelper sidelinkClient("ns3::UdpSocketFactory", socket_server);
    sidelinkClient.SetAttribute("EnableSeqTsSizeHeader", BooleanValue(true));
    std::string dataRateBeString = std::to_string(dataRateBe) + "b/s";
    sidelinkClient.SetConstantRate(DataRate(dataRateBeString), udpPacketSizeBe);

    ApplicationContainer clientApps = sidelinkClient.Install(src);
    clientApps.Start(t_finalActivationBearers);
    clientApps.Stop(t_finalSimulation);

    ApplicationContainer serverApps;
    PacketSinkHelper sidelinkSink("ns3::UdpSocketFactory", socket_client);
    sidelinkSink.SetAttribute("EnableSeqTsSizeHeader", BooleanValue(APP_EnableSeqTsSizeHeader));
    serverApps = sidelinkSink.Install(dst);
    serverApps.Start(t_serverStart);

////// UDP CLient - Server /////////////////////////////////////////////////////////////////////////
//    // Client
//    UdpClientHelper client(socket_server, port);
//    std::string dataRateBeString = std::to_string(dataRateBe) + "kb/s";
//    client.SetAttribute("MaxPackets", UintegerValue(10));
//    client.SetAttribute("Interval", TimeValue(MilliSeconds(10)));
//    client.SetAttribute("PacketSize", UintegerValue(udpPacketSizeBe)); //bytes
//
//    ApplicationContainer clientApps;
//    clientApps.Add(client.Install(src));
//    clientApps.Start(Seconds(5.0));
//    clientApps.Stop(Seconds(30.0));
//    //server
//    ApplicationContainer serverApps;
//    UdpServerHelper server(port);
//    serverApps.Add(server.Install(dst));
//    serverApps.Start(Seconds(4.0));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trace configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    SQLiteOutput db(outputDir + simTag + "-output-simulator.db");
    PacketOutputDb packetStats;
    packetStats.SetDb(&db, PacketOutputDb::PACKET_TRACE, "appStats");

    PacketOutputDb ipLayerStats;
    ipLayerStats.SetDb(&db, PacketOutputDb::PACKET_TRACE, "ipLayerStats");

    PacketOutputDb phySpectrumStats;
    phySpectrumStats.SetDb(&db, PacketOutputDb::PACKET_TRACE, "phySpectrumStats");

    std::ostringstream path;

    path.str("");
    path << "/NodeList/" << src->GetId() << "/ApplicationList/0/$ns3::OnOffApplication/TxWithAddresses";
//    path << "/NodeList/" << src->GetId() << "/ApplicationList/0/$ns3::UdpClient/TxWithAddresses";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&PacketOutputDb::SavePacketTx,&packetStats));
    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetClientTx));

    path.str("");
    path << "/NodeList/" << dst->GetId() << "/ApplicationList/0/$ns3::PacketSink/RxWithAddresses";
//    path << "/NodeList/" << dst->GetId() << "/ApplicationList/0/$ns3::UdpServer/RxWithAddresses";
    Config::ConnectWithoutContext(path.str(),MakeCallback(&PacketOutputDb::SavePacketRx,&packetStats));
    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetServerRx));

    path.str("");
    path << "/NodeList/" << dst->GetId() << "/$ns3::Ipv4L3Protocol/Rx";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&PacketOutputDb::SavePacketRxIpLayer,&ipLayerStats));

//    path.str("");
//    path << "/NodeList/" << dst->GetId() <<  "/DeviceList/*/$ns3::NrUeNetDevice/ComponentCarrierMapUe/*/NrUePhy/NrSpectrumPhyList/*/RxPsschTraceUe";
//    Config::ConnectWithoutContext(path.str(), MakeBoundCallback(&frameRx, &phySpectrumStats));

    path.str("");
    path << "/NodeList/" << src->GetId() << "/DeviceList/*/$ns3::NrUeNetDevice/ComponentCarrierMapUe/*/NrUePhy/NrSpectrumPhyList/*/TxSlDataTrace";
    Config::ConnectWithoutContext(path.str(), MakeBoundCallback(&frameTx, &phySpectrumStats));


//    path.str("");
//    path << "/NodeList/" << src->GetId() << "/ApplicationList/0/$ns3::UdpClient/TxWithAddresses";
//    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetClientTx));
//
//    path.str("");
//    path << "/NodeList/" << dst->GetId() << "/ApplicationList/0/$ns3::UdpServer/RxWithAddresses";
//    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetServerRx));

//    path.str("");
//    path << "/NodeList/" << src->GetId() << "/$ns3::Ipv4L3Protocol/LocalDeliver";
//    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetClientIpv4L3Protocol));

//    path.str("");
//    path << "/NodeList/" << router12->GetId() << "/$ns3::Ipv4L3Protocol/LocalDeliver";
//    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetClientIpv4L3Protocol));

//    path.str("");
//    path << "/NodeList/" << router12->GetId() << "/$ns3::Ipv4L3Protocol/UnicastForward";
//    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetIpForwardUnicast));

//    path.str("");
//    path << "/NodeList/" << dst->GetId() << "/$ns3::Ipv4L3Protocol/Rx";
//    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::ipv4Receive));

//    internetNodes.EnablePcapIpv4("V2X", allUes);

//    Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>(&std::cout);
//    for(uint32_t i=0; i < numUes; i++)
//    {
//        Ipv4RoutingHelper::PrintRoutingTableAt(Seconds(6), allUes.Get(i), routingStream);
//    }


    Simulator::Stop(t_finalSimulation);
    Simulator::Run();
    /*
    * VERY IMPORTANT: Do not forget to empty the database cache, which would
    * dump the data store towards the end of the simulation in to a database.
    */

    Simulator::Destroy();

    cout << endl << endl << "--- Simulation completed --- " << endl << endl;

    cout << "Packet application client sent : " << Utils::packetSent << endl;
    cout << "Packet application server received:  " << Utils::packetReceived << endl;
    double realAppStart = t_finalActivationBearers.GetSeconds() + udpPacketSizeBe / (dataRateBe);
    cout << "Real application start : " << realAppStart << " s" << endl;

    double throughputReal = Utils::packetReceived * udpPacketSizeBe * 8 / (t_finalSimulation.GetSeconds() - realAppStart) / 1000000.0;

    cout << "Throughput real        : " << throughputReal << " Mbps (bits)" << std::endl;
    cout << "Throughput theorical   : " << dataRateBe/1000000.0 << " Mbps (bits)" << std::endl;

    return 0;
}