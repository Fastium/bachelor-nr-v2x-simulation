//
// Created by ranium on 26.07.24.
//

#ifndef NS3_PACKETOUTPUTDB_H
#define NS3_PACKETOUTPUTDB_H


#include <ns3/nr-sl-phy-mac-common.h>
#include <ns3/sqlite-output.h>

#include <vector>

using namespace ns3;


class PacketOutputDb
{
    /*
     * To save :
     *   - tx/rx
     *   - time us
     *   - packet id
     *   - packet size (bytes)
     */
public:
    PacketOutputDb();
    ~PacketOutputDb() = default;
    void SetDb(SQLiteOutput* db, const std::string& tableName);

    void SavePacket(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress);

    void EmptyCache();

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

    void WriteCache();

    void Save(std::string txRx, double timeUs, uint32_t packetId, uint32_t packetSize);


    SQLiteOutput* m_db{nullptr};                 //!< DB pointer
    std::string m_tableName{"InvalidTableName"}; //!< table name
    std::vector<PacketTraceParams> m_packetTraceParamsCache;   //!< Result cache


};

#endif // NS3_PACKETOUTPUTDB_H
