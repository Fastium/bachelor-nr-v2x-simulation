//
// Created by ranium on 08.07.24.
//

#ifndef NS3_CONFIGURATION_H
#define NS3_CONFIGURATION_H

//
// Created by ranium on 21.06.24.
//

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

#include "utils.h"
#include "packetoutputdb.h"

#include <iostream>
#include <string>

using namespace std;
using namespace ns3;


// Parameters of the simulation

////////////////////////////////////////////////////////////////////////////////////////////////////
// GLobal
#define SIDELINK_DELAY                              0      // Delay of the S1u link in milliseconds
#define SHADOWING_ENABLED                          false   // Enable the shadowing model
#define SCENARIO                                  "v2v_Highway"     // Scenario
#define ERROR_MODEL                                "ns3::NrEesmCcT1" // Error model
#define SIM_TAG                                    "default"     // Simulation tag

#define NUM_ROUTERS                                          0       // Number of UEs
#define UE_DISTANCE                               50      // Distance between SRC and DST

#define SL_NUMEROLOGY_BWP                               2       // Numerology of the SL BWP
#define CENTRAL_FREQUENCY_BAND_SL                       5.89e9  // Central frequency of the SL BWP
#define BANDWIDTH_BAND_SL                               400     // Bandwidth of the SL BWP

#define NR_H_PHY_TxPower                                30      // Transmission power of the UEs
#define PHY_NOISE                                       5    // Noise power
#define PHY_LATENCY                                   100000.0     // Latency of the PHY layer (ns)
#define UPD_PACKET_SIZE                                 8000     // Size of the packet sent by the UEs (bytes)
#define DATA_RATE_BE                                    500000     // Data rate of the best effort traffic (bits per second)
#define ANTENNA_NumRows                            1       // Number of rows of the antenna
#define ANTENNA_NumColumns                         2       // Number of columns of the colomns

#define EnableSensing                          true   // Enable the sensing
#define MAC_T1                                     2       // T1
#define MAC_T2                                     33       // T2
#define MAC_ActivePoolId                           0       // Active pool ID
#define MAC_ReservationPeriod                      100       // Reservation period
#define MAC_NumSidelinkProcess                     4       // Number of sidelink process

////////////////////////////////////////////////////////////////////////////////////////////////////
// TIME
#define BEARER_ACTIVATION_TIME                          5
#define SIMULATION_TIME                                 20

//                  --Don't change--
#define BEARER_ACTIVATION_DELAY                         0.01
#define SERVER_START_TIME                               3

////////////////////////////////////////////////////////////////////////////////////////////////////
// IP
#define PORT                                            8000
#define DST_L2_ID                                       254

////////////////////////////////////////////////////////////////////////////////////////////////////
// Application
#define APP_EnableSeqTsSizeHeader                       true

////////////////////////////////////////////////////////////////////////////////////////////////////
// Don't change the following parameters
#define NUM_CC_PER_BAND                                 1       // Number of component carriers per band

// Parameters of the EPC helper
#define EPC_H_S1uLinkDelay                              0      // Delay of the S1u link in milliseconds

// Parameters of the NR helper
#define NR_H_CHANNEL_MODEL_UPDATE_PERIOD_CONDITION      0.0     // Period of the channel model update in milli seconds
#define NR_H_SHADOWING_ENABLED                          false   // Enable the shadowing model
#define NR_H_ANTENNA_NumRows                            1       // Number of rows of the antenna
#define NR_H_ANTENNA_NumColumns                         2       // Number of columns of the colomns
#define NR_H_MAC_EnableSensing                          false   // Enable the sensing
#define NR_H_MAC_T1                                     2       // T1
#define NR_H_MAC_T2                                     33       // T2
#define NR_H_MAC_ActivePoolId                           0       // Active pool ID
#define NR_H_MAC_ReservationPeriod                      100       // Reservation period
#define NR_H_MAC_NumSidelinkProcess                     4       // Number of sidelink process
#define NR_H_MAC_EnableBlindReTx                        true   // Enable the blind retransmission
#define NR_H_MANAGER_TYPE                               "ns3::NrSlBwpManagerUe" // Manager type
#define NR_H_MAC_MANAGER_GBR_MC_PUSH_TO_TALK            0       // GBR MC push to talk

// Parameters of the NR sidelink helper
#define SL_H_ERROR_MODEL                                "ns3::NrEesmIrT1" // Error model
#define SL_H_FixNrSlMcs                                 true       // Fixed MCS
#define SL_H_InitialNrSlMcs                             14     // Initial MCS

#define F_SL_TIME_RESOURCE                              100
#define F_SL_SELECTION_WINDOW                           5
#define F_SL_FREQ_RESOURCE_PSCCH                        10
#define F_SL_SUBCHANNEL_SIZE                            50
#define F_SL_MAX_NUM_PER_RESERVE                        3 // pas compris

#define SL_RESOURCE_POOL_CONFIG_NR                      true

#define SL_SYMBOL_PER_SLOTS                             14
#define SL_RB_PER_RB_GROUP                              1 // Resource block per resource block group
#define SL_NBR_SYMBOL_ALLOCATED_PER_SLOT                14 // Number of symbols allocated per slot
#define SL_FIRST_SYMBOL_ALLOCATED_IN_SLOT               0 // First symbol allocated in the slot

#define SL_BWP_GENERIC                                  true
#define SL_BWP_POOL_CONFIG_COMMON_NR                    true

#define SL_TDD_PATTERN                                  "DL|DL|DL|F|UL|UL|UL|UL|UL|UL|"

#define SL_PROB_RESOURCE_KEEP                           0

#define SL_MAX_TX_TRANS_NUM_PSSCH                       5 // Maximum number of transmission for PSSCH

//GLOBAL - STATIC
#define THREE_GPP_CHANNEL_MODEL_UPDATE_PERIOD           0.0     // Period of the channel model update in milli seconds

#endif // NS3_CONFIGURATION_H
