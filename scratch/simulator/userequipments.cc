//
// Created by ranium on 26.06.24.
//

#include "userequipments.h"

UserEquipments::UserEquipments() {

}

void UserEquipments::create(Vector *table, int nbrUe, string mobilityModel) {
    ueVoiceContainer_.Create(nbrUe);
    mobility_.SetMobilityModel(mobilityModel); //don't forget the ns3:: prefix
    Ptr<ListPositionAllocator> positionAllocUe = CreateObject<ListPositionAllocator>();
    for (int i = 0; i < nbrUe; i++) {
        positionAllocUe->Add(Vector(table[i].x, table[i].y, table[i].z));
    }
    mobility_.SetPositionAllocator(positionAllocUe);
    mobility_.Install(ueVoiceContainer_);
}

void UserEquipments::setUeVoiceNetDev(NrHelper * nrHelper, BandwidthPartInfoPtrVector * allBwps){
    this->ueVoiceNetDev_ = nrHelper->InstallUeDevice(ueVoiceContainer_, *allBwps);

    // Update the configuration of the UEs
    for (auto it = ueVoiceNetDev_.Begin(); it != ueVoiceNetDev_.End(); ++it)
    {
        DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();
    }
}

