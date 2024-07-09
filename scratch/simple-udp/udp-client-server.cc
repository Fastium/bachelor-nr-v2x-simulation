/*
 * Copyright (c) 2009 INRIA
 *
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
 *
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

// Network topology
//
//       n0    n1
//       |     |
//       =======
//         LAN (CSMA)
//
// - UDP flow from n0 to n1 of 1024 byte packets at intervals of 50 ms
//   - maximum of 320 packets sent (or limited by simulation duration)
//   - option to use IPv4 or IPv6 addressing
//   - option to disable logging statements

#include "ns3/flow-monitor-helper.h"
#include "utils.h"

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

#include <fstream>

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("UdpClientServerExample");

int
main(int argc, char* argv[])
{
    // Declare variables used in command-line arguments
    Address serverAddress;

    uint8_t serverId = 0;
    uint8_t clientId = 1;
    uint8_t hopId = 2;

//    LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
//    LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
//    LogComponentEnable("UdpClientServerExample", LOG_LEVEL_INFO);

    Packet::EnablePrinting();

    NS_LOG_INFO("Create nodes in above topology.");
    NodeContainer n;
    n.Create(3);



    InternetStackHelper internet;
    internet.Install(n);

    NS_LOG_INFO("Create channel between the two nodes.");
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", DataRateValue(DataRate(5000000)));
    csma.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));
    csma.SetDeviceAttribute("Mtu", UintegerValue(1400));
    NetDeviceContainer d = csma.Install(n);

    NS_LOG_INFO("Assign IP Addresses.");

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interface = ipv4.Assign(d);
    serverAddress = Address(interface.GetAddress(serverId));

    Ipv4StaticRoutingHelper ipv4RoutingHelper;





    NodeContainer tx;
    NodeContainer rx;
    NodeContainer hops;

    for(uint8_t i = 0; i < n.GetN(); i++) {
        if(serverId == i) {
            rx.Add(n.Get(i));
        }
        else if(clientId == i) {
            tx.Add(n.Get(i));
        }
        else if(hopId == i){
            hops.Add(n.Get(i));
        }
        else {
            cout << "Not all node are used" << endl;
        }
    }


    NS_LOG_INFO("Create UdpServer application on node 1.");
    uint16_t port = 4000;
    ApplicationContainer serversApp;
    UdpServerHelper server(port);
    serversApp.Add(server.Install(n.Get(serverId)));
    serversApp.Get(serverId)->SetStartTime(Seconds(1.0));

    NS_LOG_INFO("Create UdpClient application on node 0 to send to node 1.");
    uint32_t MaxPacketSize = 1024;
    Time interPacketInterval = Seconds(0.05);
    uint32_t maxPacketCount = 2;

    ApplicationContainer clientsApp;

    UdpClientHelper client(serverAddress, port);
    client.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    client.SetAttribute("Interval", TimeValue(interPacketInterval));
    client.SetAttribute("PacketSize", UintegerValue(MaxPacketSize));

    for(uint8_t i = 0; i < tx.GetN(); i++) {
        clientsApp.Add(client.Install(tx.Get(i)));
        clientsApp.Get(i)->SetStartTime(Seconds(2.0));
        clientsApp.Get(i)->SetStopTime(Seconds(10.0));
    }


    std::cout << "Number of server : " << rx.GetN() << std::endl;
    for(uint32_t i = 0; i < rx.GetN(); i++) {
        std::cout << "Server " << i << " address : " << rx.Get(i)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << std::endl;
    }

    std::cout << "Number of client : " << tx.GetN() << std::endl;
    for(uint32_t i = 0; i < tx.GetN(); i++) {
        std::cout << "Client " << i << " address : " << tx.Get(i)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal() << std::endl;
    }

    std::ostringstream path;

    for(uint8_t i = 0; i < tx.GetN(); i++) {
        path << "/NodeList/" << tx.Get(i)->GetId() << "/ApplicationList/0/$ns3::UdpClient/TxWithAddresses";
        Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetClientTx));
        path.str("");
    }

    for(uint8_t i = 0; i < rx.GetN(); i++) {
        path << "/NodeList/" << rx.Get(i)->GetId() << "/ApplicationList/0/$ns3::UdpServer/RxWithAddresses";
        Config::ConnectWithoutContext(path.str(), MakeCallback(&Utils::packetServerRx));
        path.str("");
    }

    internet.EnablePcapIpv4("udp-client-server", n);

    NS_LOG_INFO("Run Simulation.");
    Simulator::Run();
    Simulator::Destroy();
    NS_LOG_INFO("Done.");

    return 0;
}
