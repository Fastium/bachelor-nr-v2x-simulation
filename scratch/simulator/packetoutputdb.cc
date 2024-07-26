//
// Created by ranium on 26.07.24.
//

#include "packetoutputdb.h"

PacketOutputDb::PacketOutputDb()
{
}

void
PacketOutputDb::SetDb(SQLiteOutput* db, const std::string& tableName)
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

void
PacketOutputDb::Save(std::string txRx, double timeUs, uint32_t packetId, uint32_t packetSize)
{
    m_packetTraceParamsCache.emplace_back(PacketTraceParams(txRx, timeUs, packetId, packetSize));

    // Let's wait until ~1MB of entries before storing it in the database
    if (m_packetTraceParamsCache.size() * sizeof(PacketTraceParams) > 100000)
    {
        WriteCache();
    }
}

void
PacketOutputDb::WriteCache()
{
    bool ret = m_db->SpinExec("BEGIN TRANSACTION;");
    for (const auto& v : m_packetTraceParamsCache)
    {
        sqlite3_stmt* stmt;
        m_db->SpinPrepare(&stmt, "INSERT INTO " + m_tableName + " VALUES (?,?,?,?);");
        ret = m_db->Bind(stmt, 1, v.txRx);
        NS_ABORT_UNLESS(ret);
        ret = m_db->Bind(stmt, 2, v.timeUs);
        NS_ABORT_UNLESS(ret);
        ret = m_db->Bind(stmt, 3, v.packetId);
        NS_ABORT_UNLESS(ret);
        ret = m_db->Bind(stmt, 4, v.packetSize);
        NS_ABORT_UNLESS(ret);

        ret = m_db->SpinExec(stmt);
        NS_ABORT_UNLESS(ret);
    }

    m_packetTraceParamsCache.clear();
    ret = m_db->SpinExec("END TRANSACTION;");
    NS_ABORT_UNLESS(ret);
}

void
PacketOutputDb::EmptyCache()
{
    WriteCache();
}

void
PacketOutputDb::SavePacket(const Ptr<const Packet> packet, const Address& srcAddress, const Address& destAddress)
{
    std::cout << "PacketOutputDb::SavePacket" << std::endl;
}
