//
// Created by ranium on 26.06.24.
//

#include "network.h"

Network::Network() {

}

void Network::create() {
    bwpIdContainer_.insert(bwpIdForGbrMcptt_);
}

void Network::createCore(uint64_t msLatency) {
    core_.create(msLatency);
}

void Network::createBandwidthPart(CcBwpCreator::SimpleOperationBandConf *bandConfSl) {
    bandwidthPart_.create(bandConfSl);
}

void Network::createUserEquipments(Vector *table, int nbrUe, string mobilityModel) {
    userEquipments_.create(table, nbrUe, mobilityModel);
}

void Network::createSidelink(uint16_t numerologyBwpSl, uint16_t bandwidthBandSl) {
    configPoolBwp(numerologyBwpSl, bandwidthBandSl);
    sidelink_.create(&core_, bwpIdContainer_);
    sidelink_.updatePoolBwp(bwpIdContainer_, slBwpConfigCommonNr_);
    sidelink_.getNrSlHelper()->InstallNrSlPreConfiguration(*userEquipments_.getUeVoiceNetDev(), *sidelink_.getSlPreConfigNr());
}

void Network::createNewRadio(NewRadio::ChannelConfig_ *channelConfig, NewRadio::PhyConfig_ *phyConfig,
                             NewRadio::MacConfig_ *macConfig, NewRadio::AntennaConfig_ *antennaConfig,
                             NewRadio::BwpManagerConfig_ *bwpManagerConfig) {
    newRadio_.create(&core_, &bandwidthPart_);
    newRadio_.setChannel(channelConfig);
    newRadio_.setPhy(phyConfig);
    newRadio_.setMac(macConfig);
    newRadio_.setAntenna(antennaConfig);
    newRadio_.setBwpManager(bwpManagerConfig);


}

void Network::createIpLayer(IpLayer::configIp_t *configIp, Time finalSlBearersActivationTime) {
    ipLayer_.create(this, configIp, finalSlBearersActivationTime);
}

void Network::createApp(IpLayer *ipLayer, UserEquipments *userEquipments, App::AppConfig_t *appConfig) {
    app_.create(ipLayer, userEquipments, appConfig);
}

void Network::createTracePackets(TracePackets::TraceConfig_t *traceConfig, App *app, UserEquipments *userEquipments) {
    tracePackets_.create(userEquipments);
    tracePackets_.setupDatabase(traceConfig, app, userEquipments);
}

void Network::configPoolBwp(uint16_t numerologyBwpSl, uint16_t bandwidthBandSl) {

    this->slBitmap_ = {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1};
    this->factory_.SetSlTimeResources(slBitmap_);
    this->factory_.SetSlSensingWindow(100); // T0 in ms
    this->factory_.SetSlSelectionWindow(5);
    this->factory_.SetSlFreqResourcePscch(10); // PSCCH RBs
    this->factory_.SetSlSubchannelSize(50);
    this->factory_.SetSlMaxNumPerReserve(3);
    // Once parameters are configured, we can create the pool
    pool_ = this->factory_.CreatePool();
    slResourcePoolNr_ = pool_;

    slresoPoolConfigNr_.haveSlResourcePoolConfigNr = true;

    uint16_t poolId = 0;
    slResourcePoolIdNr_.id = poolId;
    slresoPoolConfigNr_.slResourcePoolId = slResourcePoolIdNr_;
    slresoPoolConfigNr_.slResourcePool = slResourcePoolNr_;

    slBwpPoolConfigCommonNr_.slTxPoolSelectedNormal[slResourcePoolIdNr_.id] = slresoPoolConfigNr_;

    // Configure the BWP IE
    bwp_.numerology = numerologyBwpSl;
    bwp_.symbolsPerSlots = 14;
    bwp_.rbPerRbg = 1;
    bwp_.bandwidth = bandwidthBandSl;

    // Configure the SlBwpGeneric IE
    slBwpGeneric_.bwp = bwp_;
    slBwpGeneric_.slLengthSymbols = LteRrcSap::GetSlLengthSymbolsEnum(14);
    slBwpGeneric_.slStartSymbol = LteRrcSap::GetSlStartSymbolEnum(0);

    // Configure the SlBwpConfigCommonNr IE
    slBwpConfigCommonNr_.haveSlBwpGeneric = true;
    slBwpConfigCommonNr_.slBwpGeneric = slBwpGeneric_;
    slBwpConfigCommonNr_.haveSlBwpPoolConfigCommonNr = true;
    slBwpConfigCommonNr_.slBwpPoolConfigCommonNr = slBwpPoolConfigCommonNr_;

}

void Network::emptyCache() {
    tracePackets_.emptyCache();
}







