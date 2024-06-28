//
// Created by ranium on 26.06.24.
//

#include "newradio.h"

NewRadio::NewRadio() {

}

void NewRadio::create(Core *core, BandwidthPart *bandwidthPart) {
    this->nrHelper.SetEpcHelper(core->getEpcHelper());
    this->nrHelper.InitializeOperationBand(bandwidthPart->getBandSl());
}

void NewRadio::setChannel(NewRadio::ChannelConfig_ * channelConfig) {
    nrHelper.SetChannelConditionModelAttribute("UpdatePeriod", TimeValue(MilliSeconds(channelConfig->updatePeriod)));
    nrHelper.SetPathlossAttribute("ShadowingEnabled", BooleanValue(channelConfig->shadowingEnabled));
}

void NewRadio::setPhy(NewRadio::PhyConfig_  * phyConfig) {
    nrHelper.SetUePhyAttribute("TxPower", DoubleValue(phyConfig->TxPower));
}

void NewRadio::setMac(NewRadio::MacConfig_ * macConfig) {
    nrHelper.SetUeMacAttribute("EnableSensing", BooleanValue(macConfig->EnableSensing));
    nrHelper.SetUeMacAttribute("T1", UintegerValue(macConfig->T1));
    nrHelper.SetUeMacAttribute("T2", UintegerValue(macConfig->T2));
    nrHelper.SetUeMacAttribute("ActivePoolId", UintegerValue(macConfig->ActivePoolId));
    nrHelper.SetUeMacAttribute("ReservationPeriod", TimeValue(MilliSeconds(macConfig->ReservationPeriod)));
    nrHelper.SetUeMacAttribute("NumSidelinkProcess", UintegerValue(macConfig->NumSidelinkProcess));
    nrHelper.SetUeMacAttribute("EnableBlindReTx", BooleanValue(macConfig->EnableBlindReTx));
}

void NewRadio::setAntenna(NewRadio::AntennaConfig_ * antennaConfig) {
    nrHelper.SetUeAntennaAttribute("NumRows", UintegerValue(antennaConfig->NumRows));
    nrHelper.SetUeAntennaAttribute("NumColumns", UintegerValue(antennaConfig->NumColumns));
    nrHelper.SetUeAntennaAttribute("AntennaElement",PointerValue(antennaConfig->antennaModel));

}

void NewRadio::setBwpManager(NewRadio::BwpManagerConfig_ * bwpManagerConfig) {
    nrHelper.SetBwpManagerTypeId(TypeId::LookupByName(bwpManagerConfig->lookup)); //don't forget the ns3:: prefix
    nrHelper.SetUeBwpManagerAlgorithmAttribute("GBR_MC_PUSH_TO_TALK", UintegerValue(bwpManagerConfig->GBR_MC_PUSH_TO_TALK));
}
