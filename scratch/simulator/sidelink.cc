//
// Created by ranium on 26.06.24.
//

#include "sidelink.h"

Sidelink::Sidelink() {

}

void Sidelink::create(Core *core, std::set<uint8_t> &bwpIdContainer) {
    nrSlHelper.SetEpcHelper(core->getEpcHelper());

    // Error model and the adaptive modulation coding with the MCS (AMC)
    nrSlHelper.SetSlErrorModel("ns3::NrEesmIrT1");

    // AMC -> Adaptative Modulation and Coding
    nrSlHelper.SetUeSlAmcAttribute("AmcModel", EnumValue(NrAmc::ErrorModel));

    // Sidelink scheduler attributes with fix MCS value
    nrSlHelper.SetNrSlSchedulerTypeId(NrSlUeMacSchedulerSimple::GetTypeId());
    nrSlHelper.SetUeSlSchedulerAttribute("FixNrSlMcs", BooleanValue(true));
    nrSlHelper.SetUeSlSchedulerAttribute("InitialNrSlMcs", UintegerValue(14));

    nrSlHelper.PrepareUeForSidelink(*userEquipments_->getUeVoiceNetDev(), bwpIdContainer);

}

void Sidelink::updatePoolBwp(std::set<uint8_t> &bwpIdContainer, LteRrcSap::SlBwpConfigCommonNr &slBwpConfigCommonNr) {

    for (const auto& it : bwpIdContainer)
    {
        // it is the BWP id
        slFreConfigCommonNr_.slBwpList[it] = slBwpConfigCommonNr;
    }

    // Configure the TddUlDlConfigCommon IE
    tddUlDlConfigCommon_.tddPattern = "DL|DL|DL|F|UL|UL|UL|UL|UL|UL|";

    // Configure the SlPreconfigGeneralNr IE
    slPreconfigGeneralNr_.slTddConfig = tddUlDlConfigCommon_;

    // Configure the SlPsschTxParameters IE
    psschParams_.slMaxTxTransNumPssch = 5;
    // Configure the SlPsschTxConfigList IE
    pscchTxConfigList_.slPsschTxParameters[0] = psschParams_;

    // Configure the SlUeSelectedConfig IE
    slUeSelectedPreConfig_.slPsschTxConfigList = pscchTxConfigList_;
    slUeSelectedPreConfig_.slProbResourceKeep = 0;

    slPreConfigNr_.slPreconfigGeneral = slPreconfigGeneralNr_;
    slPreConfigNr_.slUeSelectedPreConfig = slUeSelectedPreConfig_;
    slPreConfigNr_.slPreconfigFreqInfoList[0] = slFreConfigCommonNr_;

    // Communicate the above pre-configuration to the NrSlHelper
    nrSlHelper.InstallNrSlPreConfiguration(*userEquipments_->getUeVoiceNetDev(), slPreConfigNr_);
}


