#include "CDrawAScope.h"
#include <GL/freeglut.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

_Variables* _VarsDisp = _Variables::getInstance();

CDrawAScope& CDrawAScope::getInstance() {
    static CDrawAScope instance;
    return instance;
}

void CDrawAScope::setCallbacks() {
    drawGrid();
    drawAxes();
    drawLabels();
    drawDataPoints();
}

void CDrawAScope::drawAxes() {
    glColor3f(0.0f, 0.5f, 0.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    glVertex2f(_VarsDisp->xOffset, _VarsDisp->yOffset + 0.45f);
    glVertex2f(1.0f, _VarsDisp->yOffset + 0.45f);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
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
    glColor3f(0.0f, 0.8f, 0.0f);

    for (int i = 0; i <= _VarsDisp->gridYLines; ++i) {
        float y = _VarsDisp->yOffset + (float)i / _VarsDisp->gridYLines * (1.0f - _VarsDisp->yOffset);
        float amplitude = (i - _VarsDisp->gridYLines / 2) * (_VarsDisp->maxAmplitude / (_VarsDisp->gridYLines / 2));

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
    float lastY = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset);  // y=0 level

    const int numSections = 150;  // Number of sections along the x-axis
    const float sectionWidth = (1.0f - _VarsDisp->xOffset) / numSections;  // Section width based on x-axis
    _VarsDisp->detectedRange = sectionWidth / 16;  // Use detectedRange as per original logic

    std::vector<float> sectionRenderValues(numSections, lastY);  // Store render values for each section, initialized to y=0
    std::vector<float> sectionXValues(numSections, lastX);       // Store the corresponding x-values (normalizedX) for each section

    std::vector<float> positiveAmplitudes(numSections, 0.0f);  // Track max positive amplitude per section
    std::vector<float> negativeAmplitudes(numSections, 0.0f);  // Track max negative amplitude per section

    // Iterate through all data points
    for (int i = 0; i < currentSize; ++i) {
        float range_i = ranges[i];
        float amplitude_i = amplitudes[i];

        if (range_i < 0 || range_i > _VarsDisp->maxRange) continue;  // Ignore out-of-range data

        std::cout << "Amplitude[" << i << "] = " << amplitude_i << std::endl;
        std::cout << "Range[" << i << "] = " << range_i << std::endl;
        // Normalized x, y values
        float normalizedX = _VarsDisp->xOffset + (range_i / _VarsDisp->maxRange) * (1.0f - _VarsDisp->xOffset);

        // Calculate section boundaries
        float leftX = normalizedX - _VarsDisp->detectedRange;
        float rightX = normalizedX + _VarsDisp->detectedRange;

        int leftSection = std::max(0, int((leftX - _VarsDisp->xOffset) / sectionWidth));
        int rightSection = std::min(numSections - 1, int((rightX - _VarsDisp->xOffset) / sectionWidth));

        // Adjust rendering across sections for points spanning multiple sections
        for (int section = leftSection; section <= rightSection; ++section) {
            float sectionStartX = _VarsDisp->xOffset + section * sectionWidth;
            float sectionEndX = sectionStartX + sectionWidth;

            if (normalizedX >= sectionStartX && normalizedX <= sectionEndX) {
                if (amplitude_i >= 0) {
                    // Store the maximum positive amplitude
                    positiveAmplitudes[section] = std::max(positiveAmplitudes[section], amplitude_i);
                }
                else {
                    // Store the maximum (in magnitude) negative amplitude
                    negativeAmplitudes[section] = std::max(negativeAmplitudes[section], -amplitude_i);
                }
                sectionXValues[section] = normalizedX;  // Store the exact x-position of the point
            }
        }
    }

    // Calculate the final render value for each section
    for (int section = 0; section < numSections; ++section) {
        // Compute Y values for positive and negative amplitudes
        float positiveY = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset) + ((positiveAmplitudes[section] / _VarsDisp->maxAmplitude) * (1.0f - _VarsDisp->yOffset) / 2.0f);
        float negativeY = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset) - ((negativeAmplitudes[section] / _VarsDisp->maxAmplitude) * (1.0f - _VarsDisp->yOffset) / 2.0f);

        // Calculate the difference if both positive and negative amplitudes exist
        if (positiveAmplitudes[section] > 0 && negativeAmplitudes[section] > 0) {
            float positiveAmp = positiveAmplitudes[section];
            float negativeAmp = negativeAmplitudes[section];

            // Compute the difference between positive and negative amplitudes
            float amplitudeDifference = positiveAmp - negativeAmp;

            // Render the difference: if amplitudeDifference is positive, use positiveY; if negative, use negativeY
            if (amplitudeDifference >= 0) {
                sectionRenderValues[section] = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset) + ((amplitudeDifference / _VarsDisp->maxAmplitude) * (1.0f - _VarsDisp->yOffset) / 2.0f);
            }
            else {
                sectionRenderValues[section] = _VarsDisp->yOffset + 0.5f * (1.0f - _VarsDisp->yOffset) - ((-amplitudeDifference / _VarsDisp->maxAmplitude) * (1.0f - _VarsDisp->yOffset) / 2.0f);
            }
        }
        else if (positiveAmplitudes[section] > 0) {
            // Only positive value
            sectionRenderValues[section] = positiveY;
        }
        else if (negativeAmplitudes[section] > 0) {
            // Only negative value
            sectionRenderValues[section] = negativeY;
        }
    }


    // Render all sections
    glBegin(GL_LINE_STRIP);
    glVertex2f(lastX, lastY);  // Start from lastX and y=0 (lastY)

    for (int section = 0; section < numSections; ++section) {
        if (sectionRenderValues[section] != lastY) {
            // Render the point at the exact normalizedX
            glVertex2f(sectionXValues[section], sectionRenderValues[section]);
        }
        else {
            // No valid data point in this section, render at the section midpoint
            float sectionMidX = _VarsDisp->xOffset + section * sectionWidth + sectionWidth / 2.0f;
            glVertex2f(sectionMidX, lastY);
        }
    }

    float finalX = _VarsDisp->xOffset + (_VarsDisp->maxRange / _VarsDisp->maxRange) * (1.0f - _VarsDisp->xOffset);
    glVertex2f(finalX, lastY);  // Finish at finalX and y=0 (lastY)
    glEnd();
}



void CDrawAScope::addDataPoint(float range[], float amplitude[], int size) {
    currentSize = 0;

    for (int i = 0; i < size; i++) {
        if (std::abs(amplitude[i]) > _VarsDisp->maxAmplitude) {
            _VarsDisp->maxAmplitude = std::abs(amplitude[i]);
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