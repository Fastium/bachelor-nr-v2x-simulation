//
// Created by ranium on 26.06.24.
//

#include "core.h"

Core::Core() {

}

void Core::create(uint64_t msLatency) {
    this->epcHelper_ = CreateObject<NrPointToPointEpcHelper>();
    this->epcHelper_->SetAttribute("S1uLinkDelay", TimeValue(MilliSeconds(msLatency)));

}

Ptr<NrPointToPointEpcHelper> Core::getEpcHelper() const {
    return this->epcHelper_; //core network
}
