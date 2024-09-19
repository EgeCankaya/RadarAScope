#ifndef IRADARASCOPE_H
#define IRADARASCOPE_H

class IRadarAScope {
public:
    virtual void run() = 0;
    virtual void endAScope() = 0;
    virtual void addEntry(float range, float amplitude) = 0;
    virtual ~IRadarAScope() = default;
};

#endif // IRADARASCOPE_H

