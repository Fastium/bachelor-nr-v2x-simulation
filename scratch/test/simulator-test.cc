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
// Project parameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    LogComponentEnable("ScratchSimulator", LOG_LEVEL_ALL);
//    LogComponentEnable("NrSlHelper", LOG_LEVEL_ALL);
//    LogComponentEnable("LteUeRrc", LOG_LEVEL_ALL);
//    LogComponentEnable("Ipv4L3Protocol", LOG_LEVEL_ALL);
//    LogComponentEnable("UdpSocketImpl", LOG_LEVEL_ALL);

    // Where we will store the output files.
    std::string simTag = "default";
    std::string outputDir = "./";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simulator parameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Scenario parameters (that we will use inside this script):
    uint16_t interUeDistance = INTER_UE_DISTANCE; // meters

    // Traffic parameters (that we will use inside this script:)
    //bool useIPv6 = false; // default IPV4
//    uint32_t udpPacketSizeBe = UPD_PACKET_SIZE;
//    double dataRateBe = DATA_RATE_BE; // 16 kilobits per second

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
    //User Equipment
    NodeContainer ueVoiceContainer;
    uint16_t ueNum = NUM_UE;
    ueVoiceContainer.Create(ueNum);

    // Static position of the UEs
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    Ptr<ListPositionAllocator> positionAllocUe = CreateObject<ListPositionAllocator>();
    for (uint16_t i = 0; i < ueNum; i++)
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
    std::string errorModel = "ns3::NrEesmIrT1";
    nrSlHelper->SetSlErrorModel(errorModel);
    // AMC -> Adaptative Modulation and Coding
    nrSlHelper->SetUeSlAmcAttribute("AmcModel", EnumValue(NrAmc::ErrorModel));

    // Sidelink scheduler attributes with fix MCS value
    nrSlHelper->SetNrSlSchedulerTypeId(NrSlUeMacSchedulerSimple::GetTypeId());
    nrSlHelper->SetUeSlSchedulerAttribute("FixNrSlMcs", BooleanValue(true));
    nrSlHelper->SetUeSlSchedulerAttribute("InitialNrSlMcs", UintegerValue(14));

    // IMPORTANT: Prepare the UEs for sidelink
    nrSlHelper->PrepareUeForSidelink(ueVoiceNetDev, bwpIdContainer);

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
    nrSlHelper->InstallNrSlPreConfiguration(ueVoiceNetDev, slPreConfigNr);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IP configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    InternetStackHelper internet;

    uint8_t serverId = 1;
    uint8_t clientId = 0;

    // set the receiver and the transmitter
    NodeContainer txSlUes;
    NodeContainer rxSlUes;
    NetDeviceContainer txSlUesNetDevice;
    NetDeviceContainer rxSlUesNetDevice;

    txSlUes.Add(ueVoiceContainer.Get(clientId));
    rxSlUes.Add(ueVoiceContainer.Get(serverId));
    txSlUesNetDevice.Add(ueVoiceNetDev.Get(clientId));
    rxSlUesNetDevice.Add(ueVoiceNetDev.Get(serverId));

    internet.Install(ueVoiceContainer);

    // Stream configuration to randomize the models
    int64_t stream = 1;
    stream += nrHelper->AssignStreams(ueVoiceNetDev, stream);
    stream += nrSlHelper->AssignStreams(ueVoiceNetDev, stream);
    stream += internet.AssignStreams(ueVoiceContainer, stream);

    uint16_t port = 400;
    uint32_t dstL2Id = 255;
    Ptr<LteSlTft> tft;

    Ipv4StaticRoutingHelper ipv4RoutingHelper;

    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(ueVoiceNetDev);


    // set the default gateway for the UE why I need to set the default gateway for the UE
    for (uint32_t u = 0; u < ueVoiceContainer.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueVoiceContainer.Get(u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);

    }

    Ipv4Address serverAddr = ueVoiceContainer.Get(serverId)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();
//    Ipv4Address serverAddr = Ipv4Address("255.0.0.0");
//    Ipv4Address serverAddr = Ipv4Address("127.0.0.1");
//    Ipv4Address clientAddr = ueVoiceContainer.Get(clientId)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();
//    Ipv4Address localAddr = Ipv4Address("7.0.0.0");
//    Ipv4Address defaultGateway = epcHelper->GetUeDefaultGatewayAddress();

    cout << "Default gateway : " << epcHelper->GetUeDefaultGatewayAddress() << endl;
    cout << "Server address  : " << ueVoiceContainer.Get(serverId)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl ;
    cout << "Client address  : " << ueVoiceContainer.Get(clientId)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << endl ;

    InetSocketAddress remoteAddress = InetSocketAddress(serverAddr, port); // put an and point to the server
//    InetSocketAddress localAddress = InetSocketAddress(Ipv4Address::GetAny(), port); // put an end point to the client
//    InetSocketAddress localAddress = InetSocketAddress(localAddr, port); // put an end point to the client

    tft = Create<LteSlTft>(LteSlTft::Direction::RECEIVE, LteSlTft::CommType::GroupCast, serverAddr, dstL2Id);
    nrSlHelper->ActivateNrSlBearer(finalSlBearersActivationTime, ueVoiceNetDev, tft);

    tft = Create<LteSlTft>(LteSlTft::Direction::TRANSMIT, LteSlTft::CommType::GroupCast, serverAddr, dstL2Id);
    nrSlHelper->ActivateNrSlBearer(finalSlBearersActivationTime, ueVoiceNetDev, tft);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    UdpClientHelper client(remoteAddress, port);
    client.SetAttribute("MaxPackets", UintegerValue(1));
    client.SetAttribute("Interval", TimeValue(Time(100)));
    client.SetAttribute("PacketSize", UintegerValue(32));

    ApplicationContainer clientApps;
    clientApps.Add(client.Install(ueVoiceContainer.Get(clientId)));
    clientApps.Get(0)->SetStartTime(Seconds(3));
    clientApps.Get(0)->SetStopTime(Seconds(30));

    //server
    UdpServerHelper server(port);

    ApplicationContainer serverApps;
    serverApps.Add(server.Install(ueVoiceContainer.Get(serverId)));
    serverApps.Start(Seconds(0.0));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trace configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::ostringstream path;
    path << "/NodeList/" << ueVoiceContainer.Get(clientId)->GetId() << "/ApplicationList/0/$ns3::UdpClient/TxWithAddresses";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetClientTx));

    path.str("");
    path << "/NodeList/" << ueVoiceContainer.Get(serverId)->GetId() << "/ApplicationList/0/$ns3::UdpServer/RxWithAddresses";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetServerRx));

    path.str("");
    path << "/NodeList/" << ueVoiceContainer.Get(clientId)->GetId() << "/$ns3::Ipv4L3Protocol/LocalDeliver";
    Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetClientIpv4L3Protocol));

    internet.EnablePcapIpv4("test", ueVoiceContainer);

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
