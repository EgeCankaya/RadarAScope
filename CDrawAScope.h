#pragma once
#ifndef CDRAWASCOPE_H
#define CDRAWASCOPE_H

class CDrawAScope
{
public:
    static CDrawAScope& getInstance();
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
    void drawAxis();
    void drawLabels();
    void resizeArray();
    void sortDataPoints();

    float* ranges;    
    float* amplitudes;
    int currentSize;  
    int capacity;     
};   

#endif // CDRAWASCOPE_H