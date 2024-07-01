//
// Created by ranium on 21.06.24.
//

#ifndef NS3_SIMULATOR_H
#define NS3_SIMULATOR_H

#include "utils.h"

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

using namespace std;
using namespace ns3;
// Parameters of the simulation

#define NUM_UE                                      2       // Number of UEs
#define INTER_UE_DISTANCE                           20      // Distance between two UEs

#define UPD_PACKET_SIZE                             200     // Size of the packet sent by the UEs
#define DATA_RATE_BE                                16      // Data rate of the best effort traffic

#define SIMULATION_TIME                             10      // Duration of the simulation in seconds
#define SIMULATION_SL_BEARER_ACTIVATION_TIME        2.0     // Time to activate the SL bearer in seconds

#define NUMEROLOGY_BWP_SL                           2       // Numerology of the SL BWP
#define CENTRAL_FREQUENCY_BAND_SL                   5.89e9  // Central frequency of the SL BWP
#define BANDWIDTH_BAND_SL                           400     // Bandwidth of the SL BWP

#define TX_POWER                                    23      // Transmission power of the UEs

// Don't change the following parameters
#define NUM_CC_PER_BAND                             1       // Number of component carriers per band



#endif //NS3_SIMULATOR_H
