//
// Created by ranium on 26.07.24.
//

#ifndef NS3_PACKETOUTPUTDB_H
#define NS3_PACKETOUTPUTDB_H

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

#include <ns3/nr-sl-phy-mac-common.h>
#include <ns3/sqlite-output.h>

#include <vector>

using namespace ns3;


class PacketOutputDb
{
public:
    PacketOutputDb();
    ~PacketOutputDb() = default;

    enum tableNames
    {
        PACKET_TRACE,
        FRAME_TRACE
    };

    void SetDb(SQLiteOutput* db, tableNames tableName, std::string name);

    void SavePacketRx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress);
    void SavePacketTx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress);

    void SavePacketRxIpLayer(Ptr< const Packet > packet, Ptr< Ipv4 > ipv4, uint32_t interface);

    void SavePacketRxPhySpectrum(const SlRxDataPacketTraceParams traceParams);
    void SavePacketTxPhySpectrum(SlTxDataPacketTraceParams traceParams);

private:

    typedef struct PacketTraceParams_
    {
        PacketTraceParams_(std::string txRx, double timeUs, uint32_t packetId, uint32_t packetSize)
        {
            this->txRx = txRx;
            this->timeUs = timeUs;
            this->packetId = packetId;
            this->packetSize = packetSize;
        }
        std::string txRx;
        double timeUs;
        uint32_t packetId;
        uint32_t packetSize;
    }PacketTraceParams;

    typedef struct phySpectrumParams_
    {
        phySpectrumParams_(std::string txRx, double timeUs, uint32_t nodeId, bool corrupt, uint32_t frameId, uint32_t frameSize)
        {
            this->txRx = txRx;
            this->timeUs = timeUs;
            this->nodeId = nodeId;
            this->corrupt = corrupt;
            this->frameId = frameId;
            this->frameSize = frameSize;
        }
        std::string txRx;
        double timeUs;
        uint32_t nodeId;
        bool corrupt;
        uint32_t frameId;
        uint32_t frameSize;
    }phySpectrumParams;

    void Save(std::string txRx, double timeUs, uint32_t packetId, uint32_t packetSize);
    void Save(std::string txRx, double timeUs, uint32_t nodeId, bool corrupt, uint32_t frameId, uint32_t frameSize);

    SQLiteOutput* m_db{nullptr};                 //!< DB pointer
    std::string m_tableName{"InvalidTableName"}; //!< table name
    std::vector<PacketTraceParams> m_packetTraceParamsCache;   //!< Result cache


};

#endif // NS3_PACKETOUTPUTDB_H
