#ifndef CDRAWASCOPE_H
#define CDRAWASCOPE_H

#include "_globalVars.h"
#include <cstdio>

class CDrawAScope
{
public:
    void setCallbacks();
    
    CDrawAScope() : currentSize(0), capacity(10) {
        ranges = new float[capacity];
        amplitudes = new float[capacity];
    }

    ~CDrawAScope() {
        delete[] ranges;
        delete[] amplitudes;
    }

    void drawDataPoints();

    void addDataPoint(float range[], float amplitude[], int size);

private:
    void drawGrid();
    void drawAxes();
    void drawLabels();
    void resizeArray();
    void sortDataPoints();

    float* ranges;    
    float* amplitudes;
    int currentSize;  
    int capacity;     
};   
extern CDrawAScope scope;

#endif // CDRAWASCOPE_H