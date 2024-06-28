//
// Created by ranium on 26.06.24.
//

#ifndef NS3_NEWRADIO_H
#define NS3_NEWRADIO_H

#include "simulator.h"
#include "core.h"
#include "bandwidthpart.h"

class NewRadio {
public:
    typedef struct ChannelConfig {
        bool shadowingEnabled;
        uint64_t updatePeriod;
    } ChannelConfig_;

    typedef struct PhyConfig {
        double TxPower;
    } PhyConfig_;

    typedef struct MacConfig {
        bool EnableSensing;
        uint64_t T1;
        uint64_t T2;
        uint64_t ActivePoolId;
        uint64_t ReservationPeriod;
        uint64_t NumSidelinkProcess;
        bool EnableBlindReTx;
    } MacConfig_;

    typedef struct AntennaConfig {
        uint64_t NumRows;
        uint64_t NumColumns;
        AntennaModel * antennaModel;
    } AntennaConfig_;

    typedef struct BwpManagerConfig {
        std::string lookup;
        uint64_t GBR_MC_PUSH_TO_TALK;
    } BwpManagerConfig_;

    ////////////////////////////////////////////////////////////

    NewRadio();
    ~NewRadio() = default;

    void create(Core * core, BandwidthPart * bandwidthPart);

    void setChannel(ChannelConfig_ * channelConfig);
    void setPhy(PhyConfig_ * phyConfig);
    void setMac(MacConfig_ * macConfig);
    void setAntenna(AntennaConfig_ * antennaConfig);
    void setBwpManager(BwpManagerConfig_ * bwpManagerConfig);



    // Getters and Setters
    NrHelper * getNrHelper() { return &nrHelper; }
private:
    NrHelper nrHelper;

};


#endif //NS3_NEWRADIO_H
