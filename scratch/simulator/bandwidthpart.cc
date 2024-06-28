//
// Created by ranium on 26.06.24.
//

#include "bandwidthpart.h"

BandwidthPart::BandwidthPart() {
}

void BandwidthPart::create(CcBwpCreator::SimpleOperationBandConf * bandConfSl) {
    this->bandConfSl_ = bandConfSl;
    this->bandSl_ = ccBwpCreator_.CreateOperationBandContiguousCc(*bandConfSl_);
    this->allBwps_ = CcBwpCreator::GetAllBwps({bandSl_});
}

OperationBandInfo * BandwidthPart::getBandSl() {
    return &bandSl_;
}
