#ifndef CRADARASCOPE_H
#define CRADARASCOPE_H

#include "IRadarAScope.h"
#include "CDisplay.h"
#include <vector>
#include <utility>

class CRadarAScope {
public:
    CRadarAScope(IRadarAScope* display) : display(display) {}

    void run() {
        display->run();
    }

    void endAScope() {
        display->endAScope();
    }

    void addEntry(float range, float amplitude) {
        display->addEntry(range, amplitude);
    }

private:
    IRadarAScope* display;
    std::vector<std::pair<float, float>> dataPoints;
};

#endif // CRADARASCOPE_H
