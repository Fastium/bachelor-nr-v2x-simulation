//
// Created by ranium on 26.06.24.
//

#ifndef NS3_CORE_H
#define NS3_CORE_H

#include "simulator.h"

class Core {
public:
    Core();
    ~Core() = default;

    void create(uint64_t msLatency);

    // Getters and Setters
    Ptr<NrPointToPointEpcHelper> getEpcHelper() const;

private:
    Ptr<NrPointToPointEpcHelper> epcHelper_; //core network

};


#endif //NS3_CORE_H
