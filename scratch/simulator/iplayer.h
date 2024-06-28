//
// Created by ranium on 26.06.24.
//

#ifndef NS3_IPLAYER_H
#define NS3_IPLAYER_H

#include "simulator.h"
#include "network.h"


class IpLayer {
public:
    typedef struct configIp{
        uint32_t dstL2Id = 255;
        uint16_t port = 8000;
        string groupAddress = "225.255.255.0";
    }configIp_t;

    IpLayer();
    ~IpLayer() = default;

    void create(Network * network, configIp_t * configIp, Time finalSlBearersActivationTime);

    // Getters and Setters
    Address * getRemoteAddress() { return &remoteAddress_; }
    Address * getLocalAddress() { return &localAddress_; }

private:
    InternetStackHelper internetStackHelper_;
    Ptr<LteSlTft> tft_;
    Ipv4InterfaceContainer ueIpIface_;
    Ipv4StaticRoutingHelper ipv4RoutingHelper_;

    Address remoteAddress_;
    Address localAddress_;
    Ipv4Address groupAddress4_;

    int64_t stream_;
};


#endif //NS3_IPLAYER_H
