//
// Created by ranium on 26.06.24.
//

#ifndef NS3_SIDELINK_H
#define NS3_SIDELINK_H

#include "simulator.h"
#include "core.h"
#include "userequipments.h"
#include "bandwidthpart.h"


class Sidelink {
public:
    Sidelink();
    ~Sidelink() = default;

    void create(Core *core, std::set<uint8_t> &bwpIdContainer);
    void updatePoolBwp(std::set<uint8_t> &bwpIdContainer, LteRrcSap::SlBwpConfigCommonNr &slBwpConfigCommonNr);

    // Getters and Setters
    NrSlHelper * getNrSlHelper() { return &nrSlHelper; }
    LteRrcSap::SidelinkPreconfigNr * getSlPreConfigNr() { return &slPreConfigNr_; }

private:
    NrSlHelper nrSlHelper;

    LteRrcSap::SidelinkPreconfigNr slPreConfigNr_;

    LteRrcSap::TddUlDlConfigCommon tddUlDlConfigCommon_;
    LteRrcSap::SlPreconfigGeneralNr slPreconfigGeneralNr_;
    LteRrcSap::SlUeSelectedConfig slUeSelectedPreConfig_;
    LteRrcSap::SlPsschTxParameters psschParams_;
    LteRrcSap::SlPsschTxConfigList pscchTxConfigList_;
    LteRrcSap::SlFreqConfigCommonNr slFreConfigCommonNr_;

    //References
    UserEquipments * userEquipments_;

};


#endif //NS3_SIDELINK_H
