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

#include "simulator.h"

#include <iostream>
#include <iomanip>
#include "network.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ScratchSimulator"); // enable logging on terminal

int main(int argc, char* argv[])
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Configuration

    Time::SetResolution(Time::NS);

    // setup the nodes with the NR stack
    Packet::EnableChecking();
    Packet::EnablePrinting();

    Config::SetDefault("ns3::ThreeGppChannelModel::UpdatePeriod", TimeValue(MilliSeconds(100)));

    // Simulation parameters.
    Time simTime = Seconds(SIMULATION_TIME);
    // Sidelink bearers activation time
    Time slBearersActivationTime = Seconds(SIMULATION_SL_BEARER_ACTIVATION_TIME);
    // Final simulation time is the addition of:
    // simTime + slBearersActivationTime + 10 ms additional delay for UEs to activate the bearers
    Time finalSlBearersActivationTime = slBearersActivationTime + Seconds(0.01);
    Time finalSimTime = simTime + finalSlBearersActivationTime;
    std::cout << "Final Simulation duration " << finalSimTime.GetSeconds() << std::endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Parameters

    //Bandwidth part info
    CcBwpCreator::SimpleOperationBandConf bandConfSl(CENTRAL_FREQUENCY_BAND_SL,
                                                     BANDWIDTH_BAND_SL,
                                                     NUM_CC_PER_BAND,
                                                     BANDWIDTH_PART_INFO_SCENARIO);

    //Channel config
    NewRadio::ChannelConfig_ channelConfig;
    channelConfig.shadowingEnabled = false;
    channelConfig.updatePeriod = 0;

    //Phy config
    NewRadio::PhyConfig_ phyConfig;
    phyConfig.TxPower = TX_POWER;

    //Mac config
    NewRadio::MacConfig_ macConfig;
    macConfig.EnableSensing = false;
    macConfig.T1 = 2;
    macConfig.T2 = 33;
    macConfig.ActivePoolId = 0;
    macConfig.ReservationPeriod = 100;
    macConfig.NumSidelinkProcess = 4;
    macConfig.EnableBlindReTx = true;

    //Antenna config
    NewRadio::AntennaConfig_ antennaConfig;
    antennaConfig.NumRows = 1;
    antennaConfig.NumColumns = 1;
    antennaConfig.antennaModel = new IsotropicAntennaModel();

    //Bwp manager config
    NewRadio::BwpManagerConfig_ bwpManagerConfig;
    bwpManagerConfig.lookup = "ns3::NrSlBwpManagerUe";
    bwpManagerConfig.GBR_MC_PUSH_TO_TALK = 0;

    //Ip layer config
    IpLayer::configIp_t configIp;
    configIp.dstL2Id = 255;
    configIp.port = 8000;
    configIp.groupAddress = "225.255.255.0";

    // App config
    App::AppConfig_t appConfig;
    appConfig.EnableSeqTsSizeHeader = true;
    appConfig.dataRateBe = DATA_RATE_BE;
    appConfig.udpPacketSizeBe = UPD_PACKET_SIZE;
    appConfig.finalSlBearersActivationTime = finalSlBearersActivationTime;
    appConfig.finalSimTime = finalSimTime;

    // Trace config
    TracePackets::TraceConfig_t traceConfig;
    traceConfig.outputDir = "./outputs/";
    traceConfig.simTag = "default";

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Physical architecture

    Vector physicalArchitecture[NUM_UE];
    physicalArchitecture[0] = Vector(0, 0, 0);
    physicalArchitecture[1] = Vector(INTER_UE_DISTANCE, 0, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Network
    Network network;
    network.createCore(0);
    network.createUserEquipments(physicalArchitecture, NUM_UE, "ns3::ConstantPositionMobilityModel");
    network.createBandwidthPart(&bandConfSl);
    network.createSidelink(NUMEROLOGY_BWP_SL, BANDWIDTH_BAND_SL);
    network.createNewRadio(&channelConfig, &phyConfig, &macConfig, &antennaConfig, &bwpManagerConfig);
    network.createIpLayer(&configIp, finalSlBearersActivationTime);
    network.createApp(network.getIpLayer(), network.getUserEquipments(), &appConfig);
    network.createTracePackets(&traceConfig, network.getApp(), network.getUserEquipments());

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Simulation
    Simulator::Stop(finalSimTime);
    Simulator::Run();

    std::cout << "Total Tx bits = " << TracePackets::txByteCounter * 8 << std::endl;
    std::cout << "Total Tx packets = " << TracePackets::txPktCounter << std::endl;

    std::cout << "Total Rx bits = " << TracePackets::rxByteCounter * 8 << std::endl;
    std::cout << "Total Rx packets = " << TracePackets::rxPktCounter << std::endl;

    std::cout << "Avrg thput = "
              << (Utils::rxByteCounter * 8) / (finalSimTime - Seconds(realAppStart)).GetSeconds() / 1000.0
              << " kbps" << std::endl;

    std::cout << "Average Packet Inter-Reception (PIR) " << TracePackets::pir.GetSeconds() / TracePackets::pirCounter << " sec"
              << std::endl;

    /*
     * VERY IMPORTANT: Do not forget to empty the database cache, which would
     * dump the data store towards the end of the simulation in to a database.
     */
    network.emptyCache();

    Simulator::Destroy();


    return 0;
}
