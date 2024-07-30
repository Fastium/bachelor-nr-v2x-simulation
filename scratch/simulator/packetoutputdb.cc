//
// Created by ranium on 26.07.24.
//

#include "packetoutputdb.h"

PacketOutputDb::PacketOutputDb()
{
}

void
PacketOutputDb::SetDbPacket(SQLiteOutput* db, const std::string& tableName)
{
    m_db = db;
    m_tableName = tableName;

    bool ret;

    ret = db->SpinExec("CREATE TABLE IF NOT EXISTS " + tableName +
                       " ("
                       "TxRx TEXT NOT NULL,"
                       "TimeUs DOUBLE NOT NULL,"
                       "PacketId INTEGER NOT NULL,"
                       "PacketSize INTEGER NOT NULL"
                       ");");

    NS_ABORT_UNLESS(ret);


}

void PacketOutputDb::SavePacket(std::string txRx, double timeUs, uint32_t packetId, uint32_t packetSize)
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

void PacketOutputDb::SaveFrame(std::string txRx, double timeUs, uint32_t nodeId, bool corrupt, uint32_t frameId, uint32_t frameSize)
{
    bool ret = m_db->SpinExec("BEGIN TRANSACTION;");

    sqlite3_stmt* stmt;

    m_db->SpinPrepare(&stmt, "INSERT INTO " + m_tableName + " VALUES (?,?,?,?,?,?,?);");
    ret = m_db->Bind(stmt, 1, txRx);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 2, timeUs);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 3, nodeId);
    NS_ABORT_UNLESS(ret);
    ret = m_db->Bind(stmt, 4, corrupt);
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
    this->SavePacket(txRx, Simulator::Now().GetNanoSeconds(), p_id, packet->GetSize());
}

void PacketOutputDb::SavePacketTx(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    std::string txRx = "Tx";
    uint32_t p_id = packet->GetUid();
//    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
//    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
//    std::cout << "Packet (" << p_id << ") client Tx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
    this->SavePacket(txRx, Simulator::Now().GetNanoSeconds(), p_id, packet->GetSize());
}

void
PacketOutputDb::SavePacketRxIpLayer(Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface)
{
    std::string txRx = "Rx";
    uint32_t p_id = packet->GetUid();
//    Ipv4Address src = ipv4->GetSourceAddress();
//    Ipv4Address dest = ipv4->GetDestinationAddress();
//    std::cout << "Packet (" << p_id << ") server Rx: " << packet->GetSize() << " bytes from " <<  src << " to " << dest << std::endl;
    this->SavePacket(txRx, Simulator::Now().GetNanoSeconds(), p_id, packet->GetSize());
}

void PacketOutputDb::SavePacketRxPhySpectrum(SlRxDataPacketTraceParams traceParams)
{
    std::string txRx = "Rx";
    uint32_t node_id = traceParams.m_ndi;
    traceParams.m_corrupt;
    traceParams.m_frameNum;

    this->Save(txRx, Simulator::Now().GetNanoSeconds(), , traceParams.packetSize);
}



