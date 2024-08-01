//
// Created by ranium on 26.07.24.
//

#include "packetoutputdb.h"

PacketOutputDb::PacketOutputDb()
{
}

void
PacketOutputDb::SetDb(SQLiteOutput* db, tableNames tableName, std::string name)
{
    m_db = db;
    m_tableName = name;

    std::ostringstream cmd;
    cmd.str("");

    bool ret;
    switch(tableName)
    {
        case PACKET_TRACE:
            cmd << "CREATE TABLE IF NOT EXISTS " << name <<
                  "(TxRx TEXT NOT NULL,"
                  "TimeUs DOUBLE NOT NULL,"
                  "PacketId INTEGER NOT NULL,"
                  "PacketSize INTEGER NOT NULL"
                  ");";
            std::cout << cmd.str() << std::endl;
            break;
        case FRAME_TRACE:
            cmd << "CREATE TABLE IF NOT EXISTS " << name <<
                  "(TxRx TEXT NOT NULL,"
                  "TimeUs DOUBLE NOT NULL,"
                  "NodeId INTEGER NOT NULL,"
                  "Corrupt INTEGER NOT NULL,"
                  "FrameId INTEGER NOT NULL,"
                  "FrameSize INTEGER NOT NULL"
                  ");";
            break;
        default:
            NS_FATAL_ERROR("Invalid table name");
    }

    ret = db->SpinExec(cmd.str());

    NS_ABORT_UNLESS(ret);


}

void PacketOutputDb::Save(std::string txRx, double timeUs, uint32_t packetId, uint32_t packetSize)
{
    bool ret = m_db->SpinExec("BEGIN TRANSACTION;");

    sqlite3_stmt* stmt;

    m_db->SpinPrepare(&stmt, "INSERT INTO " + m_tableName + " VALUES (?,?,?,?);");
    ret = m_db->Bind(stmt, 1, txRx);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 2, timeUs);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 3, packetId);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 4, packetSize);
    NS_ABORT_UNLESS(ret);

    ret = m_db->SpinExec(stmt);
    NS_ABORT_UNLESS(ret);

    ret = m_db->SpinExec("END TRANSACTION;");
    NS_ABORT_UNLESS(ret);

}

void PacketOutputDb::Save(std::string txRx, double timeUs, uint32_t nodeId, bool corrupt, uint32_t frameId, uint32_t frameSize)
{
    bool ret = m_db->SpinExec("BEGIN TRANSACTION;");

    sqlite3_stmt* stmt;

    m_db->SpinPrepare(&stmt, "INSERT INTO " + m_tableName + " VALUES (?,?,?,?,?,?);");
    ret = m_db->Bind(stmt, 1, txRx);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 2, timeUs);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 3, nodeId);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 4, corrupt?1:0);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 5, frameId);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 6, frameSize);
    NS_ABORT_UNLESS(ret);

    ret = m_db->SpinExec(stmt);
    NS_ABORT_UNLESS(ret);

    ret = m_db->SpinExec("END TRANSACTION;");
    NS_ABORT_UNLESS(ret);
}

void PacketOutputDb::SavePacketRx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    std::string txRx = "Rx";
    uint32_t p_id = packet->GetUid();
//    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
//    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
//    std::cout << "Packet (" << p_id << ") server Rx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
    this->Save(txRx, Simulator::Now().GetNanoSeconds(), p_id, packet->GetSize());
}

void PacketOutputDb::SavePacketTx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    std::string txRx = "Tx";
    uint32_t p_id = packet->GetUid();
//    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
//    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
//    std::cout << "Packet (" << p_id << ") client Tx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
    this->Save(txRx, Simulator::Now().GetNanoSeconds(), p_id, packet->GetSize());
}

void
PacketOutputDb::SavePacketRxIpLayer(Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface)
{
    std::string txRx = "Rx";
    uint32_t p_id = packet->GetUid();
//    Ipv4Address src = ipv4->GetSourceAddress();
//    Ipv4Address dest = ipv4->GetDestinationAddress();
//    std::cout << "Packet (" << p_id << ") server Rx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
    this->Save(txRx, Simulator::Now().GetNanoSeconds(), p_id, packet->GetSize());
}

void PacketOutputDb::SavePacketRxPhySpectrum(const SlRxDataPacketTraceParams traceParams)
{
    std::string txRx = "Rx";
    uint32_t node_id = traceParams.m_ndi;
    double timeUs = Simulator::Now().GetNanoSeconds();
    this->Save(txRx, timeUs, node_id, traceParams.m_corrupt, traceParams.m_frameNum, traceParams.m_tbSize);
}

void PacketOutputDb::SavePacketTxPhySpectrum(const SlTxDataPacketTraceParams traceParams)
{
    std::string txRx = "Tx";
    std::list<Ptr<Packet>> pbList = traceParams.pb->GetPackets();
    std::list<Ptr<Packet>>::iterator it;

    for(it = pbList.begin(); it != pbList.end(); it++)
    {
        this->Save(txRx, Simulator::Now().GetNanoSeconds(), (*it)->GetUid(), (*it)->GetSize());
    }
}
