//
// Created by ranium on 26.06.24.
//

#ifndef NS3_USEREQUIPMENTS_H
#define NS3_USEREQUIPMENTS_H

#include "simulator.h"


using namespace ns3;

class UserEquipments {

public:
    UserEquipments();
    ~UserEquipments() = default;

    void create(Vector * table, int nbrUe, string mobilityModel);

    // Getters and Setters
    void setUeVoiceNetDev(NrHelper * nrHelper, BandwidthPartInfoPtrVector * allBwps);
    NetDeviceContainer * getUeVoiceNetDev() { return &this->ueVoiceNetDev_; }
    NodeContainer * getUeVoiceContainer() { return &this->ueVoiceContainer_;}


private:
    NodeContainer ueVoiceContainer_;
    MobilityHelper mobility_;
    NetDeviceContainer ueVoiceNetDev_;

};


#endif //NS3_USEREQUIPMENTS_H
