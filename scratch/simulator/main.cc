//
// Created by ranium on 08.07.24.
//

#include "configuration.h"

#include <iomanip>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("V2X-simulator"); // enable logging on terminal

int main(int argc, char* argv[])
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Project parameters
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //    LogComponentEnable("ScratchSimulator", LOG_LEVEL_ALL);
    //    LogComponentEnable("NrSlHelper", LOG_LEVEL_ALL);
    //    LogComponentEnable("LteUeRrc", LOG_LEVEL_ALL);
//    LogComponentEnable("NrNetDevice", LOG_LEVEL_ALL);


    // Where we will store the output files.
    std::string simTag = "default";
    std::string outputDir = "./";

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Simulator parameters
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Scenario parameters (that we will use inside this script):
    //uint16_t srcDstDistance = 200; // meters
//    uint16_t numUes = 1;

    // Traffic parameters (that we will use inside this script:)
    uint32_t udpPacketSizeBe = UPD_PACKET_SIZE;
    double dataRateBe = DATA_RATE_BE; // 16 kilobits per second

    // Simulation parameters.
    Time simTime = Seconds(SIMULATION_TIME);
    // Sidelink bearers activation time
    Time slBearersActivationTime = Seconds(SIMULATION_SL_BEARER_ACTIVATION_TIME);

    // NR parameters. We will take the input from the command line, and then we
    // will pass them inside the NR module.
    uint16_t numerologyBwpSl = SL_NUMEROLOGY_BWP;
    double centralFrequencyBandSl = CENTRAL_FREQUENCY_BAND_SL; // band n47  TDD //Here band is analogous to channel
    uint16_t bandwidthBandSl = BANDWIDTH_BAND_SL;         // Multiple of 100 KHz; 400 = 40 MHz
    double txPower = NR_H_PHY_TxPower;                    // dBm

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
    Ptr<Node> src = CreateObject<Node>();
    Ptr<Node> dst = CreateObject<Node>();

    Ptr<Node> router12 = CreateObject<Node>();

    NodeContainer routers(router12);
    NodeContainer nodes(src, dst);
    NodeContainer allUes;

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    Ptr<ListPositionAllocator> positionAllocUe = CreateObject<ListPositionAllocator>();

    positionAllocUe->Add(Vector(0, 0.0, 20));
    positionAllocUe->Add(Vector(100, 0.0, 20));
    positionAllocUe->Add(Vector(200, 0.0, 20));

    allUes.Add(src, routers, dst);

    NodeContainer network1(src, router12);
    NodeContainer network2(router12, dst);

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

    BandwidthPartInfo::Scenario bandwidthPartScenario = BandwidthPartInfo::Scenario::UMi_StreetCanyon;

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
    Config::SetDefault("ns3::ThreeGppChannelModel::UpdatePeriod", TimeValue(MilliSeconds(100)));
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
    epcHelper->SetAttribute("S1uLinkDelay", TimeValue(MilliSeconds(0)));

    //antenna configuration quasi-omnidirectional transmission and reception (default configuration of the beams)
    nrHelper->SetUeAntennaAttribute("NumRows", UintegerValue(1));
    nrHelper->SetUeAntennaAttribute("NumColumns", UintegerValue(2));
    nrHelper->SetUeAntennaAttribute("AntennaElement",PointerValue(CreateObject<IsotropicAntennaModel>()));

    // Physical layer configuration
    nrHelper->SetUePhyAttribute("TxPower", DoubleValue(txPower));

    // Mac layer configuration
    nrHelper->SetUeMacAttribute("EnableSensing", BooleanValue(false));
    nrHelper->SetUeMacAttribute("T1", UintegerValue(2));
    nrHelper->SetUeMacAttribute("T2", UintegerValue(33));
    nrHelper->SetUeMacAttribute("ActivePoolId", UintegerValue(0));
    nrHelper->SetUeMacAttribute("ReservationPeriod", TimeValue(MilliSeconds(100)));
    nrHelper->SetUeMacAttribute("NumSidelinkProcess", UintegerValue(4));
    nrHelper->SetUeMacAttribute("EnableBlindReTx", BooleanValue(true));

    uint8_t bwpIdForGbrMcptt = 0;

    nrHelper->SetBwpManagerTypeId(TypeId::LookupByName("ns3::NrSlBwpManagerUe"));
    // following parameter has no impact at the moment because:
    // 1. No support for PQI based mapping between the application and the LCs
    // 2. No scheduler to consider PQI
    // However, till such time all the NR SL examples should use GBR_MC_PUSH_TO_TALK
    // because we hard coded the PQI 65 in UE RRC.
    nrHelper->SetUeBwpManagerAlgorithmAttribute("GBR_MC_PUSH_TO_TALK",
                                               UintegerValue(bwpIdForGbrMcptt));

    std::set<uint8_t> bwpIdContainer;
    bwpIdContainer.insert(bwpIdForGbrMcptt);

    ////////////////////////////////////////////////////////////
    // 2. Attributes valid for a subset of the nodes
    // NOT PRESENT IN THIS SIMPLE EXAMPLE

    /*
    * We have configured the attributes we needed. Now, install and get the pointers
    * to the NetDevices, which contains all the NR stack:
    */
    NetDeviceContainer net1Container = nrHelper->InstallUeDevice(network1, allBwps);
    NetDeviceContainer net2Container = nrHelper->InstallUeDevice(network2, allBwps);
    NetDeviceContainer allNetDevices(net1Container, net2Container);

    ////////////////////////////////////////////////////////////////////////
    // 3. Go node for node and change the attributes we have to set up per-node.
    // NOT PRESENT IN THIS SIMPLE EXAMPLE


    // update all configuration for the UE devices
    for (auto it = allNetDevices.Begin(); it != allNetDevices.End(); ++it)
    {
       DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();
    }

    // Sidelink configuration
    Ptr<NrSlHelper> nrSlHelper = CreateObject<NrSlHelper>();
    nrSlHelper->SetEpcHelper(epcHelper);

    // Error model and the adaptive modulation coding with the MCS (AMC)
    std::string errorModel = "ns3::NrEesmIrT1";
    nrSlHelper->SetSlErrorModel(errorModel);
    // AMC -> Adaptative Modulation and Coding
    nrSlHelper->SetUeSlAmcAttribute("AmcModel", EnumValue(NrAmc::ErrorModel));

    // Sidelink scheduler attributes with fix MCS value
    nrSlHelper->SetNrSlSchedulerTypeId(NrSlUeMacSchedulerSimple::GetTypeId());
    nrSlHelper->SetUeSlSchedulerAttribute("FixNrSlMcs", BooleanValue(true));
    nrSlHelper->SetUeSlSchedulerAttribute("InitialNrSlMcs", UintegerValue(14));

    // IMPORTANT: Prepare the UEs for sidelink
    nrSlHelper->PrepareUeForSidelink(allNetDevices, bwpIdContainer);

    // Resource pool configuration
    LteRrcSap::SlResourcePoolNr slResourcePoolNr;
    Ptr<NrSlCommPreconfigResourcePoolFactory> ptrFactory = Create<NrSlCommPreconfigResourcePoolFactory>();

    std::vector<std::bitset<1>> slBitmap = {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1};

    ptrFactory->SetSlTimeResources(slBitmap);
    ptrFactory->SetSlSensingWindow(100); // T0 in ms
    ptrFactory->SetSlSelectionWindow(5);
    ptrFactory->SetSlFreqResourcePscch(10); // PSCCH RBs
    ptrFactory->SetSlSubchannelSize(50);
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
    bwp.rbPerRbg = 1;
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
    tddUlDlConfigCommon.tddPattern = "DL|DL|DL|F|UL|UL|UL|UL|UL|UL|";

    // Configure the SlPreconfigGeneralNr IE
    LteRrcSap::SlPreconfigGeneralNr slPreconfigGeneralNr;
    slPreconfigGeneralNr.slTddConfig = tddUlDlConfigCommon;

    // Configure the SlUeSelectedConfig IE
    LteRrcSap::SlUeSelectedConfig slUeSelectedPreConfig;
    slUeSelectedPreConfig.slProbResourceKeep = 0;
    // Configure the SlPsschTxParameters IE
    LteRrcSap::SlPsschTxParameters psschParams;
    psschParams.slMaxTxTransNumPssch = 5;
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
    stream += nrHelper->AssignStreams(net1Container, stream);
    stream += nrHelper->AssignStreams(net2Container, stream);
    stream += nrSlHelper->AssignStreams(net1Container, stream);
    stream += nrSlHelper->AssignStreams(net2Container, stream);

    RipHelper ripRouting;

    Ipv4ListRoutingHelper listRH;
    listRH.Add(ripRouting, 0);

    InternetStackHelper internet;
    internet.SetIpv6StackInstall(false);
    internet.SetRoutingHelper(listRH);
    internet.Install(routers);
//    stream += internet.AssignStreams(routers, stream);


    InternetStackHelper internetNodes;
    internetNodes.SetIpv6StackInstall(false);
    internetNodes.Install(nodes);
//    stream += internetNodes.AssignStreams(nodes, stream);

    // Assign addresses.
    // The source and destination networks have global addresses
    // The "core" network just needs link-local addresses for routing.
    // We assign global addresses to the routers as well to receive
    // ICMPv6 errors.
    NS_LOG_INFO("Assign IPv4 Addresses.");
    Ipv4AddressHelper ipv4;

    ipv4.SetBase(Ipv4Address("10.0.0.0"), Ipv4Mask("255.255.255.0"));
    Ipv4InterfaceContainer iic1 = ipv4.Assign(net1Container);

    ipv4.SetBase(Ipv4Address("10.0.1.0"), Ipv4Mask("255.255.255.0"));
    Ipv4InterfaceContainer iic2 = ipv4.Assign(net2Container);

    uint16_t port = 8000;
    Ptr<LteSlTft> tft;

    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> staticRouting;
    staticRouting = Ipv4RoutingHelper::GetRouting<Ipv4StaticRouting>(src->GetObject<Ipv4>()->GetRoutingProtocol());
    staticRouting->SetDefaultRoute("10.0.0.2", 1);
    staticRouting = Ipv4RoutingHelper::GetRouting<Ipv4StaticRouting>(dst->GetObject<Ipv4>()->GetRoutingProtocol());
    staticRouting->SetDefaultRoute("10.0.1.2", 1);

    cout << "src: " << src->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl;
    cout << "dst: " << dst->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl;
    cout << "router12 interface 1: " << router12->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl;
    cout << "router12 interface 2: " << router12->GetObject<Ipv4>()->GetAddress(2, 0).GetLocal() << endl;

//    cout << "N0-I1 : " << allUes.Get(0)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl;
//    cout << "N1-I0 : " << allUes.Get(1)->GetObject<Ipv4>()->GetAddress(0, 0).GetLocal() << endl;
//    cout << "N1-I1 : " << allUes.Get(1)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl;
//    cout << "N1-I2 : " << allUes.Get(1)->GetObject<Ipv4>()->GetAddress(2, 0).GetLocal() << endl;
//    cout << "N2-I1 : " << allUes.Get(2)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl;

    Ipv4Address addr2("10.0.1.2");
    Address remoteAddr2 = InetSocketAddress(addr2, port);

    tft = Create<LteSlTft>(LteSlTft::Direction::TRANSMIT, LteSlTft::CommType::Broadcast, addr2, 255);
    nrSlHelper->ActivateNrSlBearer(finalSlBearersActivationTime, net1Container, tft);

    tft = Create<LteSlTft>(LteSlTft::Direction::RECEIVE, LteSlTft::CommType::Broadcast, addr2, 255);
    nrSlHelper->ActivateNrSlBearer(finalSlBearersActivationTime, net1Container, tft);

    tft = Create<LteSlTft>(LteSlTft::Direction::TRANSMIT, LteSlTft::CommType::Broadcast, addr2, 255);
    nrSlHelper->ActivateNrSlBearer(finalSlBearersActivationTime, net2Container, tft);

    tft = Create<LteSlTft>(LteSlTft::Direction::RECEIVE, LteSlTft::CommType::Broadcast, addr2, 255);
    nrSlHelper->ActivateNrSlBearer(finalSlBearersActivationTime, net2Container, tft);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Application configuration
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::string dataRateBeString = std::to_string(dataRateBe) + "kb/s";
//    std::cout << "Data rate : " << DataRate(dataRateBeString) << std::endl;

    // Client
    UdpClientHelper client(remoteAddr2, port);
    client.SetAttribute("MaxPackets", UintegerValue(1));
    client.SetAttribute("Interval", TimeValue(Time(100)));
    client.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps;

    double realAppStart = 0.0;
    double realAppStopTime = 0.0;
//    double txAppDuration = 0.0;

    clientApps.Add(client.Install(src));
    clientApps.Get(0)->SetStartTime(slBearersActivationTime);
    realAppStart = slBearersActivationTime.GetSeconds() + ((double)udpPacketSizeBe * 8.0 / (DataRate(dataRateBeString).GetBitRate()));
    realAppStopTime = realAppStart + simTime.GetSeconds();
    clientApps.Get(0)->SetStopTime(Seconds(realAppStopTime));
//    txAppDuration = realAppStopTime - realAppStart;

    // Output app start, stop and duration
//    std::cout << "Start time " << std::fixed << std::setprecision(5) << realAppStart << " sec" << std::endl;
//    std::cout << "Stop time " << realAppStopTime << " sec" << std::endl;
//    std::cout << "Tx App duration " << std::defaultfloat << txAppDuration << " sec" << std::endl;

    //server
    ApplicationContainer serverApps;
    UdpServerHelper server(port);
    serverApps.Add(server.Install(dst));
    serverApps.Start(Seconds(0.0));

//    uint32_t packetSize = 1024;
//    Time interPacketInterval = Seconds(1.0);
//    PingHelper ping(remoteAddr2);
//
//    ping.SetAttribute("Interval", TimeValue(interPacketInterval));
//    ping.SetAttribute("Size", UintegerValue(packetSize));
//    ping.SetAttribute("VerboseMode", EnumValue(Ping::VerboseMode::VERBOSE));
//
//    ApplicationContainer apps = ping.Install(src);
//    apps.Start(Seconds(1.0));
//    apps.Stop(Seconds(10.0));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trace configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//    std::ostringstream path;
//    path << "/NodeList/" << ueVoiceContainer.Get(clientId)->GetId() << "/ApplicationList/0/$ns3::OnOffApplication/TxWithAddresses";
//    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetClientTx));
//
//    path.str("");
//
//    path << "/NodeList/" << ueVoiceContainer.Get(serverId)->GetId() << "/ApplicationList/0/$ns3::PacketSink/RxWithAddresses";
//    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetServerRx));

    internet.EnablePcapIpv4("V2X", allUes);

    Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>(&std::cout);
    Ipv4RoutingHelper::PrintRoutingTableAt(Seconds(5.0), router12, routingStream);
    Ipv4RoutingHelper::PrintRoutingTableAt(Seconds(5.0), src, routingStream);
    Ipv4RoutingHelper::PrintRoutingTableAt(Seconds(5.0), dst, routingStream);

    Simulator::Stop(finalSimTime);
    Simulator::Run();
    /*
    * VERY IMPORTANT: Do not forget to empty the database cache, which would
    * dump the data store towards the end of the simulation in to a database.
    */

    Simulator::Destroy();

    std::cout << endl << endl << "--- Simulation completed --- " << endl;
    return 0;
}