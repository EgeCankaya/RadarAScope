#include "CDrawAScope.h"
#include <GL/freeglut.h>
#include <iostream>
#include <algorithm>

void CDrawAScope::setCallbacks() {
    drawGrid();
    drawAxes();
    drawLabels();
    drawDataPoints();
}

void CDrawAScope::drawGrid() {                               
    glColor3f(0.0f, 0.5f, 0.0f);
    glLineWidth(1.0f);

    for (int i = 0; i <= gridXLines; ++i) {
        float x = xOffset + (float)i / gridXLines * (1.0f - xOffset);
        glBegin(GL_LINES);
        glVertex2f(x, yOffset);
        glVertex2f(x, 1.0f);
        glEnd();
    }

    for (int i = 0; i <= gridYLines; ++i) {
        float y = yOffset + (float)i / gridYLines * (1.0f - yOffset);
        glBegin(GL_LINES);
        glVertex2f(xOffset, y);
        glVertex2f(1.0f, y);
        glEnd();
    }
}

void CDrawAScope::drawAxes() {
    glColor3f(0.0f, 0.5f, 0.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    glVertex2f(xOffset, yOffset + 0.45f); 
    glVertex2f(1.0f, yOffset + 0.45f);  
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(xOffset, yOffset);
    glVertex2f(xOffset, 1.0f);
    glEnd();
}


void CDrawAScope::drawLabels() {
    glColor3f(0.0f, 0.8f, 0.0f);

    for (int i = 0; i <= gridYLines; ++i) {
        float y = yOffset + (float)i / gridYLines * (1.0f - yOffset);
        float amplitude = (i - gridYLines / 2) * (maxAmplitude / (gridYLines / 2));

        glPushMatrix();
        glTranslatef(xOffset - 0.05f, y - 0.01f, 0.0f);
        glScalef(0.00015f, 0.00015f, 1.0f);
        char label[10];
        sprintf_s(label, "%.2f", amplitude);
        for (char* c = label; *c != '\0'; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
    }
    float currentWindowStart = (int(rangeShift) * maxRange);
    float currentWindowEnd = currentWindowStart + maxRange;

    for (int i = 0; i <= gridXLines; ++i) {
        float x = xOffset + (float)i / gridXLines * (1.0f - xOffset);
        float range = currentWindowStart + i * (maxRange / gridXLines);

        glPushMatrix();
        glTranslatef(x - 0.02f, yOffset - 0.05f, 0.0f);
        glScalef(0.00015f, 0.00015f, 1.0f);

        char label[10];
        sprintf_s(label, "%.0f", range);
        for (char* c = label; *c != '\0'; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
    }
}



/*void CDrawAScope::drawDataPoints(const std::vector<std::pair<float, float>>& points) {
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    float currentWindowStart = (int(rangeShift) * maxRange);
    float currentWindowEnd = currentWindowStart + maxRange;

    glBegin(GL_LINE_STRIP);
    for (const auto& point : points) {
        if (point.first >= currentWindowStart && point.first <= currentWindowEnd) {
            float normalizedX = xOffset + ((point.first - currentWindowStart) / maxRange) * (1.0f - xOffset);
            float normalizedY = yOffset + (point.second / maxAmplitude) * (1.0f - yOffset);
            glVertex2f(normalizedX, normalizedY);
        }
    }
    glEnd();
}
*/

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
    glColor3f(0.0f, 1.0f, 0.0f);  
    glLineWidth(1.5f);            

    float lastX = xOffset; 
    float lastY = yOffset + 0.5f * (1.0f - yOffset);  

    for (int i = 0; i < currentSize; ++i) {
        float range = ranges[i];
        float amplitude = amplitudes[i];

        if (range >= 0 && range <= maxRange) {
            float normalizedX = xOffset + (range / maxRange) * (1.0f - xOffset);
            float normalizedY = yOffset + ((amplitude / maxAmplitude) * (1.0f - yOffset) / 2.0f + 0.5f * (1.0f - yOffset));

            float numPoints = ((maxRange / xLabelInterval) * 2) + 1;
            float pointInterval = xLabelInterval / 2;
            float distBetweenGrids = (1.0f - xOffset) / gridXLines;
            detectedRange = distBetweenGrids / 4;

            float leftX = normalizedX - detectedRange;
            float rightX = normalizedX + detectedRange;

            bool overlapDetected = false;
            if (i + 1 < currentSize) {
                float nextRange = ranges[i + 1];
                float nextAmplitude = amplitudes[i + 1];
                float nextNormalizedX = xOffset + (nextRange / maxRange) * (1.0f - xOffset);
                float nextNormalizedY = yOffset + ((nextAmplitude / maxAmplitude) * (1.0f - yOffset) / 2.0f + 0.5f * (1.0f - yOffset));
                float nextLeftX = nextNormalizedX - detectedRange;

                if (rightX > nextLeftX) {
                    overlapDetected = true;

                    if (amplitude >= nextAmplitude) {
                        glBegin(GL_LINE_STRIP);
                        glVertex2f(leftX, lastY);  
                        glVertex2f(normalizedX, normalizedY); 
                        glVertex2f(nextNormalizedX, nextNormalizedY);  
                        glVertex2f(nextNormalizedX + detectedRange, lastY);
                        glEnd();
                    }
                    else {
                        // Skip rendering 
                        continue;
                    }
                }
            }

          
            if (!overlapDetected) {
            
                glBegin(GL_LINES);
                glVertex2f(lastX, lastY); 
                glVertex2f(leftX, lastY);  
                glEnd();

                if (amplitude > 0) {
                    glBegin(GL_LINE_STRIP);
                    glVertex2f(leftX, lastY);
                    glVertex2f(normalizedX, normalizedY);
                    glVertex2f(rightX, lastY);
                    glEnd();
                }


                glBegin(GL_LINES);
                glVertex2f(rightX, lastY);
                lastX = rightX;
                lastY = yOffset + 0.5f * (1.0f - yOffset);       
                glVertex2f(lastX, lastY); 
                glEnd();
            }
        }
    }


    float finalX = xOffset + (maxRange / maxRange) * (1.0f - xOffset);
    glBegin(GL_LINES);
    glVertex2f(lastX, lastY);   
    glVertex2f(finalX, lastY);
    glEnd();
}





void CDrawAScope::addDataPoint(float range, float amplitude) {
    float tolerance = 0.05f;

    if (std::abs(amplitude) > maxAmplitude + tolerance) {
        maxAmplitude = std::abs(amplitude);
    }

    if (currentSize >= capacity) {
        resizeArray();
    }

    ranges[currentSize] = range;
    amplitudes[currentSize] = amplitude;
    ++currentSize;

    sortDataPoints();

    glutPostRedisplay();
}

CDrawAScope scope;