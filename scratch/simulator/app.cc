//
// Created by ranium on 26.06.24.
//

#include "app.h"

App::App() {

}

void App::create(IpLayer *ipLayer, UserEquipments * userEquipments, App::AppConfig_t *appConfig) {
    sidelinkClient_ = new OnOffHelper("ns3::UdpSocketFactory", *ipLayer->getLocalAddress());


    sidelinkClient_->SetAttribute("EnableSeqTsSizeHeader", BooleanValue(appConfig->EnableSeqTsSizeHeader));
    std::string dataRateBeString = std::to_string(appConfig->dataRateBe) + "kb/s";
    std::cout << "Data rate " << DataRate(dataRateBeString) << std::endl;
    sidelinkClient_->SetConstantRate(DataRate(dataRateBeString), appConfig->udpPacketSizeBe);

    clientApps_ = sidelinkClient_->Install(userEquipments->getUeVoiceContainer()->Get(0));

    realAppStart_ = appConfig->finalSlBearersActivationTime.GetSeconds() + ((double)appConfig->udpPacketSizeBe * 8.0 / (DataRate(dataRateBeString).GetBitRate()));
    realAppStop_ = (appConfig->finalSimTime).GetSeconds();

    std::cout << "App start time " << realAppStart_ << " sec" << std::endl;
    std::cout << "App stop time " << realAppStop_ << " sec" << std::endl;

    sidelinkSink_ = new PacketSinkHelper("ns3::UdpSocketFactory", *ipLayer->getRemoteAddress());
    sidelinkSink_->SetAttribute("EnableSeqTsSizeHeader", BooleanValue(true));
    serverApps_ = sidelinkSink_->Install(userEquipments->getUeVoiceContainer()->Get(userEquipments->getUeVoiceContainer()->GetN() - 1));
    serverApps_.Start(Seconds(2.0));

}

