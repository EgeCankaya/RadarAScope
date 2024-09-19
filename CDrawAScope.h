#ifndef CDRAWASCOPE_H
#define CDRAWASCOPE_H

#include "_globalVars.h"
#include <cstdio>

class CDrawAScope
{
public:
    void setCallbacks();
    void addDataPoint(float range, float amplitude);
    
    CDrawAScope() : currentSize(0), capacity(10) {
        ranges = new float[capacity];
        amplitudes = new float[capacity];
    }

    ~CDrawAScope() {
        delete[] ranges;
        delete[] amplitudes;
    }

    void drawDataPoints();

private:
    void drawGrid();
    void drawAxes();
    void drawLabels();
    void resizeArray();
    void sortDataPoints();

    float* ranges;       // Dynamic array for range values
    float* amplitudes;   // Dynamic array for amplitude values
    int currentSize;     // Number of data points added
    int capacity;        // Current capacity of the dynamic arrays
};   
extern CDrawAScope scope;

#endif // CDRAWASCOPE_H