//
// Created by ranium on 21.06.24.
//

#ifndef BACHELOR_NS3_SIMULATOR_UTILS_HPP
#define BACHELOR_NS3_SIMULATOR_UTILS_HPP

#include "ue-mac-pscch-tx-output-stats.h"
#include "ue-mac-pssch-tx-output-stats.h"
#include "ue-phy-pscch-rx-output-stats.h"
#include "ue-phy-pssch-rx-output-stats.h"
#include "ue-to-ue-pkt-txrx-output-stats.h"

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

using namespace ns3;

class Utils {
public:
    /*
     * Variables to count TX/RX packets and bytes.
     */
    static uint32_t rxByteCounter; //!< Global variable to count RX bytes
    static uint32_t txByteCounter; //!< Global variable to count TX bytes
    static uint32_t rxPktCounter;  //!< Global variable to count RX packets
    static uint32_t txPktCounter;  //!< Global variable to count TX packets

    /*
     * Variable used to compute PIR
     */
    static uint64_t pirCounter; //!< counter to count how many time we computed the PIR. It is used to compute average PIR
    static Time lastPktRxTime; //!< Global variable to store the RX time of a packet
    static Time pir;           //!< Global varible to store PIR value

    /**
     * \brief Method to listen the trace SlPscchScheduling of NrUeMac, which gets
     *        triggered upon the transmission of SCI format 1-A from UE MAC.
     *
     * \param pscchStats Pointer to the UeMacPscchTxOutputStats class,
     *        which is responsible to write the trace source parameters to a database.
     * \param pscchStatsParams Parameters of the trace source.
     */
    static void NotifySlPscchScheduling(UeMacPscchTxOutputStats* pscchStats, const SlPscchUeMacStatParameters pscchStatsParams);
    /**
     * \brief Method to listen the trace SlPsschScheduling of NrUeMac, which gets
     *        triggered upon the transmission of SCI format 2-A and data from UE MAC.
     *
     * \param psschStats Pointer to the UeMacPsschTxOutputStats class,
     *        which is responsible to write the trace source parameters to a database.
     * \param psschStatsParams Parameters of the trace source.
     */
    static void NotifySlPsschScheduling(UeMacPsschTxOutputStats* psschStats, const SlPsschUeMacStatParameters psschStatsParams);
    /**
     * \brief Method to listen the trace RxPscchTraceUe of NrSpectrumPhy, which gets
     *        triggered upon the reception of SCI format 1-A.
     *
     * \param pscchStats Pointer to the UePhyPscchRxOutputStats class,
     *        which is responsible to write the trace source parameters to a database.
     * \param pscchStatsParams Parameters of the trace source.
     */
    static void NotifySlPscchRx(UePhyPscchRxOutputStats* pscchStats, const SlRxCtrlPacketTraceParams pscchStatsParams);
    /**
     * \brief Method to listen the trace RxPsschTraceUe of NrSpectrumPhy, which gets
     *        triggered upon the reception of SCI format 2-A and data.
     *
     * \param psschStats Pointer to the UePhyPsschRxOutputStats class,
     *        which is responsible to write the trace source parameters to a database.
     * \param psschStatsParams Parameters of the trace source.
     */
    static void NotifySlPsschRx(UePhyPsschRxOutputStats* psschStats, const SlRxDataPacketTraceParams psschStatsParams);
    /**
     * \brief Method to listen the application level traces of type TxWithAddresses
     *        and RxWithAddresses.
     * \param stats Pointer to the UeToUePktTxRxOutputStats class,
     *        which is responsible to write the trace source parameters to a database. *
     * \param node The pointer to the TX or RX node
     * \param localAddrs The local IPV4 address of the node
     * \param txRx The string indicating the type of node, i.e., TX or RX
     * \param p The packet
     * \param srcAddrs The source address from the trace
     * \param dstAddrs The destination address from the trace
     * \param seqTsSizeHeader The SeqTsSizeHeader
     */
    static void UePacketTraceDb(UeToUePktTxRxOutputStats* stats,
                         Ptr<Node> node,
                         const Address& localAddrs,
                         std::string txRx,
                         Ptr<const Packet> p,
                         const Address& srcAddrs,
                         const Address& dstAddrs,
                         const SeqTsSizeHeader& seqTsSizeHeader);

    /**
     * \brief Method to listen the packet sink application trace Rx.
     * \param packet The packet
     */
    static void ReceivePacket(Ptr<const Packet> packet, const Address&);

    /**
     * \brief Method to listen the transmitting application trace Tx.
     * \param packet The packet
     */
    static void TransmitPacket(Ptr<const Packet> packet);

    /**
     * \brief This method listens to the packet sink application trace Rx.
     * \param packet The packet
     * \param from The address of the transmitter
     */
    static void ComputePir(Ptr<const Packet> packet, const Address&);

    static void packetClientTx(const Ptr< const Packet > packet, const Address &srcAddress, const Address &destAddress);

    static void packetServerRx(const Ptr< const Packet > packet, const Address &srcAddress, const Address &destAddress);

    static void packetClientRx(const Ptr< const Packet > packet, const Address &srcAddress, const Address &destAddress);

private:
    Utils() = default;
    ~Utils() = default;

};


#endif //BACHELOR_NS3_SIMULATOR_UTILS_HPP
