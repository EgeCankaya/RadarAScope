#include "CDrawAScope.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <vector>
#include "_globalVars.h"

_Variables* _VarsDisp = _Variables::getInstance();

CDrawAScope& CDrawAScope::getInstance() {
    static CDrawAScope instance;
    return instance;
}

void CDrawAScope::setCallbacks() {
    drawGrid();
    drawLabels();
    drawAxis();
    drawDataPoints();
}

void CDrawAScope::drawAxis() {
    glColor3f(0.5f, 0.0f, 0.0f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);
    glVertex2f(_VarsDisp->xOffset, _VarsDisp->yOffset + 0.45f);
    glVertex2f(1.0f, _VarsDisp->yOffset + 0.45f);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(_VarsDisp->xOffset, _VarsDisp->yOffset);
    glVertex2f(_VarsDisp->xOffset, 1.0f);
    glEnd();
}

void CDrawAScope::drawGrid() {                               
    glColor3f(0.0f, 0.3f, 0.0f);
    glLineWidth(1.0f);

    for (int i = 0; i <= _VarsDisp->gridXLines; ++i) {
        float x = _VarsDisp->xOffset + (float)i / _VarsDisp->gridXLines * (1.0f - _VarsDisp->xOffset);
        glBegin(GL_LINES);
        glVertex2f(x, _VarsDisp->yOffset);
        glVertex2f(x, 1.0f);
        glEnd();
    }

    for (int i = 0; i <= _VarsDisp->gridYLines; ++i) {
        float y = _VarsDisp->yOffset + (float)i / _VarsDisp->gridYLines * (1.0f - _VarsDisp->yOffset);
        glBegin(GL_LINES);
        glVertex2f(_VarsDisp->xOffset, y);
        glVertex2f(1.0f, y);
        glEnd();
    }
}

void CDrawAScope::drawLabels() {
    glColor3f(0.0f, 0.7f, 0.0f);

    for (int i = 0; i <= _VarsDisp->gridYLines; ++i) {
        float y = _VarsDisp->yOffset + (float)i / _VarsDisp->gridYLines * (1.0f - _VarsDisp->yOffset);
        float amplitude = (i - _VarsDisp->gridYLines / static_cast<float>(2)) * (_VarsDisp->maxAmplitude / (_VarsDisp->gridYLines / 2));

        float xOffsetAdjustment = (amplitude < 0.0f) ? -0.065f : -0.05f;

        glPushMatrix();
        glTranslatef(_VarsDisp->xOffset + xOffsetAdjustment, y - 0.01f, 0.0f);
        glScalef(0.00015f, 0.00015f, 1.0f);

        char label[10];
        sprintf_s(label, "%.1f", amplitude);

        for (char* c = label; *c != '\0'; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }

        glPopMatrix();
    }

    float currentWindowStart = (int(_VarsDisp->rangeShift) * _VarsDisp->maxRange);
    float currentWindowEnd = currentWindowStart + _VarsDisp->maxRange;

    for (int i = 0; i <= _VarsDisp->gridXLines; ++i) {
        float x = _VarsDisp->xOffset + (float)i / _VarsDisp->gridXLines * (1.0f - _VarsDisp->xOffset);
        float range = currentWindowStart + i * (_VarsDisp->maxRange / _VarsDisp->gridXLines);

        glPushMatrix();
        glTranslatef(x - 0.02f, _VarsDisp->yOffset - 0.05f, 0.0f);
        glScalef(0.00015f, 0.00015f, 1.0f);

        char label[10];
        sprintf_s(label, "%.0f", range);
        for (char* c = label; *c != '\0'; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }

        glPopMatrix();
    }
}

void CDrawAScope::resizeArray() {
    capacity *= 2;
    float* newRanges = new float[capacity];
    float* newAmplitudes = new float[capacity];

    for (int i = 0; i < currentSize; ++i) {
        newRanges[i] = ranges[i];        
        newAmplitudes[i] = amplitudes[i];
    }
                          
    delete[] ranges;      
    delete[] amplitudes;  
    ranges = newRanges;   
    amplitudes = newAmplitudes;
}

void CDrawAScope::sortDataPoints() {
    int* indices = new int[currentSize];
    for (int i = 0; i < currentSize; ++i) {
        indices[i] = i;
    }

    auto comparator = [this](int a, int b) {
        return ranges[a] < ranges[b];
        };

    std::sort(indices, indices + currentSize, comparator);

    float* sortedRanges = new float[capacity];
    float* sortedAmplitudes = new float[capacity];

    for (int i = 0; i < currentSize; ++i) {
        sortedRanges[i] = ranges[indices[i]];
        sortedAmplitudes[i] = amplitudes[indices[i]];
    }

    delete[] ranges;
    delete[] amplitudes;
    ranges = sortedRanges;
    amplitudes = sortedAmplitudes;

    delete[] indices; 
}

void CDrawAScope::drawDataPoints() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);

    float lastX = _VarsDisp->xOffset;
    float lastY = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset); 

    const int numSections = 150;
    const float sectionWidth = (1.0f - _VarsDisp->xOffset) / numSections; 
    _VarsDisp->detectedRange = sectionWidth / 16;

    std::vector<float> sectionRenderValues(numSections, lastY);  
    std::vector<float> sectionXValues(numSections, lastX);  

    std::vector<float> positiveAmplitudes(numSections, 0.0f);
    std::vector<float> negativeAmplitudes(numSections, 0.0f);

    for (int i = 0; i < currentSize; ++i) {
        float range_i = ranges[i];
        float amplitude_i = amplitudes[i];

        if (range_i < 0 || range_i > _VarsDisp->maxRange) continue;
        float normalizedX = _VarsDisp->xOffset + (range_i / _VarsDisp->maxRange) * (1.0f - _VarsDisp->xOffset);

        float leftX = normalizedX - _VarsDisp->detectedRange;
        float rightX = normalizedX + _VarsDisp->detectedRange;

        int leftSection = std::max(0, int((leftX - _VarsDisp->xOffset) / sectionWidth));
        int rightSection = std::min(numSections - 1, int((rightX - _VarsDisp->xOffset) / sectionWidth));

        for (int section = leftSection; section <= rightSection; ++section) {
            float sectionStartX = _VarsDisp->xOffset + section * sectionWidth;
            float sectionEndX = sectionStartX + sectionWidth;

            if (normalizedX >= sectionStartX && normalizedX <= sectionEndX) {
                if (amplitude_i >= 0) {
                    positiveAmplitudes[section] = std::max(positiveAmplitudes[section], amplitude_i);
                }
                else {
                    negativeAmplitudes[section] = std::max(negativeAmplitudes[section], -amplitude_i);
                }
                sectionXValues[section] = normalizedX;
            }
        }
    }

    for (int section = 0; section < numSections; ++section) {
        float positiveY = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset) + ((positiveAmplitudes[section] / _VarsDisp->maxAmplitude) * (1.0f - _VarsDisp->yOffset) / 2.0f);
        float negativeY = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset) - ((negativeAmplitudes[section] / _VarsDisp->maxAmplitude) * (1.0f - _VarsDisp->yOffset) / 2.0f);

        if (positiveAmplitudes[section] > 0 && negativeAmplitudes[section] > 0) {
            float positiveAmp = positiveAmplitudes[section];
            float negativeAmp = negativeAmplitudes[section];

            float amplitudeDifference = positiveAmp - negativeAmp;

            if (amplitudeDifference >= 0) {
                sectionRenderValues[section] = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset) + ((amplitudeDifference / _VarsDisp->maxAmplitude) * (1.0f - _VarsDisp->yOffset) / 2.0f);
            }
            else {
                sectionRenderValues[section] = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset) - ((-amplitudeDifference / _VarsDisp->maxAmplitude) * (1.0f - _VarsDisp->yOffset) / 2.0f);
            }
        }
        else if (positiveAmplitudes[section] > 0) {
            sectionRenderValues[section] = positiveY;
        }
        else if (negativeAmplitudes[section] > 0) {
            sectionRenderValues[section] = negativeY;
        }
    }

    glBegin(GL_LINE_STRIP);
    glVertex2f(lastX, lastY); 

    for (int section = 0; section < numSections; ++section) {
        if (sectionRenderValues[section] != lastY) {
            glVertex2f(sectionXValues[section], sectionRenderValues[section]);
        }
        else {
            float sectionMidX = _VarsDisp->xOffset + section * sectionWidth + sectionWidth / 2.0f;
            glVertex2f(sectionMidX, lastY);
        }
    }

    float finalX = _VarsDisp->xOffset + (_VarsDisp->maxRange / _VarsDisp->maxRange) * (1.0f - _VarsDisp->xOffset);
    glVertex2f(finalX, lastY);
    glEnd();
}

void CDrawAScope::addDataPoint(float range[], float amplitude[], int size) {
    currentSize = 0;

    _VarsDisp->maxAmplitude = 0.0f;

    for (int i = 0; i < size; i++) {
        float absAmplitude = std::abs(amplitude[i]);

        if (absAmplitude > _VarsDisp->maxAmplitude) {
            _VarsDisp->maxAmplitude = absAmplitude;
        }

        if (currentSize >= capacity) {
            resizeArray();
        }

        ranges[currentSize] = range[i];
        amplitudes[currentSize] = amplitude[i];
        ++currentSize;
    }

    sortDataPoints();
    glutPostRedisplay();
}