//
// Created by ranium on 26.06.24.
//

#ifndef NS3_APP_H
#define NS3_APP_H

#include "simulator.h"
#include "iplayer.h"
#include "userequipments.h"


class App {
public:
    typedef struct AppConfig{
        bool EnableSeqTsSizeHeader = true;
        uint64_t dataRateBe;
        uint64_t udpPacketSizeBe;
        Time finalSlBearersActivationTime;
        Time finalSimTime;
    }AppConfig_t;

    App();
    ~App() = default;

    void create(IpLayer * ipLayer, UserEquipments * userEquipments, AppConfig_t * appConfig);

    // Getters and Setters
    ApplicationContainer * getClientApps() { return &this->clientApps_; }
    ApplicationContainer * getServerApps() { return &this->serverApps_; }

private:
    ApplicationContainer clientApps_;
    ApplicationContainer serverApps_;
    OnOffHelper *  sidelinkClient_;
    PacketSinkHelper * sidelinkSink_;

    double realAppStart_;
    double realAppStop_;

};


#endif //NS3_APP_H
