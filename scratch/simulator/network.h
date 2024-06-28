//
// Created by ranium on 26.06.24.
//

#ifndef NS3_NETWORK_H
#define NS3_NETWORK_H

#include "simulator.h"
#include "core.h"
#include "bandwidthpart.h"
#include "userequipments.h"
#include "iplayer.h"
#include "sidelink.h"
#include "newradio.h"
#include "app.h"
#include "tracepackets.h"

//class IpLayer;
//class App;
//class TracePackets;


class Network {
public:
    Network();
    ~Network() = default;
    void create();
    void createCore(uint64_t msLatency);
    void createBandwidthPart(CcBwpCreator::SimpleOperationBandConf * bandConfSl);
    void createUserEquipments(Vector * table, int nbrUe, string mobilityModel);
    void createSidelink(uint16_t numerologyBwpSl, uint16_t bandwidthBandSl);
    void createNewRadio(NewRadio::ChannelConfig_ * channelConfig,
                        NewRadio::PhyConfig_ * phyConfig,
                        NewRadio::MacConfig_ * macConfig,
                        NewRadio::AntennaConfig_ * antennaConfig,
                        NewRadio::BwpManagerConfig_ * bwpManagerConfig);
    void createIpLayer(IpLayer::configIp_t * configIp, Time finalSlBearersActivationTime);
    void createApp(IpLayer * ipLayer, UserEquipments * userEquipments, App::AppConfig_t * appConfig);
    void createTracePackets(TracePackets::TraceConfig_t * traceConfig, App * app, UserEquipments * userEquipments);

    void emptyCache();

    // Getters and Setters
    Core * getCore() { return &core_; }
    BandwidthPart * getBandwidthPart() { return &bandwidthPart_; }
    UserEquipments * getUserEquipments() { return &userEquipments_; }
    IpLayer * getIpLayer() { return &ipLayer_; }
    Sidelink * getSidelink() { return &sidelink_; }
    NewRadio * getNewRadio() { return &newRadio_; }

private:
    void configPoolBwp(uint16_t numerologyBwpSl, uint16_t bandwidthBandSl);


private:
    Core core_;
    BandwidthPart bandwidthPart_;
    UserEquipments userEquipments_;
    IpLayer ipLayer_;
    Sidelink sidelink_;
    NewRadio newRadio_;
    App app_;
    TracePackets tracePackets_;

    //resource pool and bwp for sidelink
    LteRrcSap::Bwp bwp_;
    LteRrcSap::SlBwpGeneric slBwpGeneric_;
    NrSlCommPreconfigResourcePoolFactory factory_;
    LteRrcSap::SlResourcePoolIdNr slResourcePoolIdNr_;
    LteRrcSap::SlResourcePoolConfigNr slresoPoolConfigNr_;
    LteRrcSap::SlBwpPoolConfigCommonNr slBwpPoolConfigCommonNr_;
    LteRrcSap::SlResourcePoolNr slResourcePoolNr_;
    LteRrcSap::SlResourcePoolNr pool_;

    LteRrcSap::SlBwpConfigCommonNr slBwpConfigCommonNr_; //contains all configuration

    //configuration variables
    uint8_t bwpIdForGbrMcptt_;
    std::vector<std::bitset<1>> slBitmap_;
    std::set<uint8_t> bwpIdContainer_;






};


#endif //NS3_NETWORK_H
