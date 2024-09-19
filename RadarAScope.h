#ifndef RADARASCOPE_H
#define RADARASCOPE_H

#include "IRadarAScope.h"
#include <vector>
#include <utility>

class CRadarAScopeDisplay : public IRadarAScope {
public:
    void run() override;
    void endAScope() override;
    void addEntry(float range, float amplitude) override;
private:
    std::vector<std::pair<float, float>> dataPoints;
};

#endif // RADARASCOPE_H
