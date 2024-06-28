//
// Created by ranium on 26.06.24.
//

#ifndef NS3_BANDWIDTHPART_H
#define NS3_BANDWIDTHPART_H

#include "simulator.h"


class BandwidthPart {
public:
    BandwidthPart();
    ~BandwidthPart() = default;

    void create(CcBwpCreator::SimpleOperationBandConf * bandConfSl);

    OperationBandInfo * getBandSl();

private:
    BandwidthPartInfoPtrVector allBwps_;
    CcBwpCreator ccBwpCreator_;

    CcBwpCreator::SimpleOperationBandConf * bandConfSl_;
    OperationBandInfo bandSl_;

};


#endif //NS3_BANDWIDTHPART_H
