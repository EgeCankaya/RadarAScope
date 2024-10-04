#include "CDrawAScope.h"
#include <GL/freeglut.h>
#include <iostream>
#include <algorithm>
#include <cmath>

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
    glVertex2f(xOffset, yOffset + 0.45f); 
    glVertex2f(1.0f, yOffset + 0.45f);  
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(xOffset, yOffset);
    glVertex2f(xOffset, 1.0f);
    glEnd();
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
void CDrawAScope::drawLabels() {
    glColor3f(0.0f, 0.8f, 0.0f);

    for (int i = 0; i <= gridYLines; ++i) {
        float y = yOffset + (float)i / gridYLines * (1.0f - yOffset);
        float amplitude = (i - gridYLines / 2) * (maxAmplitude / (gridYLines / 2));

        float xOffsetAdjustment = (amplitude < 0.0f) ? -0.065f : -0.05f;

        glPushMatrix();
        glTranslatef(xOffset + xOffsetAdjustment, y - 0.01f, 0.0f);
        glScalef(0.00015f, 0.00015f, 1.0f);

        char label[10];
        sprintf_s(label, "%.1f", amplitude);

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
    glLineWidth(1.0f);

    float lastX = xOffset;
    float lastY = yOffset + 0.5f * (1.0f - yOffset);
    std::cout << currentSize << std::endl;

    for (int i = 0; i < currentSize - 1; ++i) {
        float range = ranges[i];
        float amplitude = amplitudes[i];

        float range_next = ranges[i + 1];
        float amplitude_next = amplitudes[i + 1];

        float range_next_next = ranges[i + 2];
        float amplitude_next_next = amplitudes[i + 2];

        if (range >= 0 && range <= maxRange) {
            float normalizedX = xOffset + (range / maxRange) * (1.0f - xOffset);
            float normalizedY = yOffset + ((amplitude / maxAmplitude) * (1.0f - yOffset) / 2.0f + 0.5f * (1.0f - yOffset));

            float normalizedX_next = xOffset + (range_next / maxRange) * (1.0f - xOffset);
            float normalizedY_next = yOffset + ((amplitude_next / maxAmplitude) * (1.0f - yOffset) / 2.0f + 0.5f * (1.0f - yOffset));

            float normalizedX_next_next = xOffset + (range_next_next / maxRange) * (1.0f - xOffset);
            float normalizedY_next_next = yOffset + ((amplitude_next_next / maxAmplitude) * (1.0f - yOffset) / 2.0f + 0.5f * (1.0f - yOffset));

            float distBetweenGrids = (1.0f - xOffset) / gridXLines;
            detectedRange = distBetweenGrids / 8;

            float leftX = normalizedX - detectedRange;
            float rightX = normalizedX + detectedRange;

            float leftX_next = normalizedX_next - detectedRange;
            float rightX_next = normalizedX_next + detectedRange;

            float leftX_next_next = normalizedX_next_next - detectedRange;
            float rightX_next_next = normalizedX_next_next + detectedRange;

            int num_points = 40;

            float edge_points_x[80];       // 2 * num_points
            float edge_points_y[80];       // 2 * num_points

            float edge_points_x_next[80];  // 2 * num_points
            float edge_points_y_next[80];  // 2 * num_points

            float edge_points_x_next_next[80];  // 2 * num_points
            float edge_points_y_next_next[80];  // 2 * num_points

            for (int n = 0; n < num_points; ++n) {
                float t_left = static_cast<float>(n) / (num_points - 1);
                edge_points_x[n] = leftX + t_left * (normalizedX - leftX);
                edge_points_y[n] = lastY + t_left * (normalizedY - lastY);

                float t_right = static_cast<float>(n) / (num_points - 1);
                edge_points_x[num_points + n] = normalizedX + t_right * (rightX - normalizedX);
                edge_points_y[num_points + n] = normalizedY + t_right * (lastY - normalizedY);
            }

            for (int n = 0; n < num_points; ++n) {
                float t_left = static_cast<float>(n) / (num_points - 1);
                edge_points_x_next[n] = leftX_next + t_left * (normalizedX_next - leftX_next);
                edge_points_y_next[n] = lastY + t_left * (normalizedY_next - lastY);

                float t_right = static_cast<float>(n) / (num_points - 1);
                edge_points_x_next[num_points + n] = normalizedX_next + t_right * (rightX_next - normalizedX_next);
                edge_points_y_next[num_points + n] = normalizedY_next + t_right * (lastY - normalizedY_next);
            }

            for (int n = 0; n < num_points; ++n) {
                float t_left = static_cast<float>(n) / (num_points - 1);
                edge_points_x_next_next[n] = leftX_next_next + t_left * (normalizedX_next_next - leftX_next);
                edge_points_y_next_next[n] = lastY + t_left * (normalizedY_next_next - lastY);

                float t_right = static_cast<float>(n) / (num_points - 1);
                edge_points_x_next_next[num_points + n] = normalizedX_next_next + t_right * (rightX_next_next - normalizedX_next_next);
                edge_points_y_next_next[num_points + n] = normalizedY_next_next + t_right * (lastY - normalizedY_next_next);
            }

            glBegin(GL_LINE_STRIP);
            bool intersection = false;
            bool intersection_next = false;
            int intersection_ID = 0;
            int intersection_ID_next = 0;

            for (int n = 0; n < 2 * num_points; ++n) {
                for (int j = 0; j < num_points; ++j) {
                    if (std::abs(edge_points_x[n] - edge_points_x_next[j]) < 0.015f && std::abs(edge_points_y[n] - edge_points_y_next[j]) < 0.015f) {
                        intersection = true;
                        intersection_ID = j;
                    }
                }

                if (!intersection) {
                    glVertex2f(edge_points_x[n], edge_points_y[n]);
                }
                else {
                    break;
                }
            }
            do {
                if (i + 2 >= currentSize) break;
                range_next = ranges[i + 1];
                amplitude_next = amplitudes[i + 1];

                range_next_next = ranges[i + 2];
                amplitude_next_next = amplitudes[i + 2];

                normalizedX_next = xOffset + (range_next / maxRange) * (1.0f - xOffset);
                normalizedY_next = yOffset + ((amplitude_next / maxAmplitude) * (1.0f - yOffset) / 2.0f + 0.5f * (1.0f - yOffset));

                normalizedX_next_next = xOffset + (range_next_next / maxRange) * (1.0f - xOffset);
                normalizedY_next_next = yOffset + ((amplitude_next_next / maxAmplitude) * (1.0f - yOffset) / 2.0f + 0.5f * (1.0f - yOffset));

                leftX_next = normalizedX_next - detectedRange;
                rightX_next = normalizedX_next + detectedRange;

                leftX_next_next = normalizedX_next_next - detectedRange;
                rightX_next_next = normalizedX_next_next + detectedRange;

                for (int n = 0; n < num_points; ++n) {
                    float t_left = static_cast<float>(n) / (num_points - 1);
                    edge_points_x_next[n] = leftX_next + t_left * (normalizedX_next - leftX_next);
                    edge_points_y_next[n] = lastY + t_left * (normalizedY_next - lastY);

                    float t_right = static_cast<float>(n) / (num_points - 1);
                    edge_points_x_next[num_points + n] = normalizedX_next + t_right * (rightX_next - normalizedX_next);
                    edge_points_y_next[num_points + n] = normalizedY_next + t_right * (lastY - normalizedY_next);
                }

                for (int n = 0; n < num_points; ++n) {
                    float t_left = static_cast<float>(n) / (num_points - 1);
                    edge_points_x_next_next[n] = leftX_next_next + t_left * (normalizedX_next_next - leftX_next);
                    edge_points_y_next_next[n] = lastY + t_left * (normalizedY_next_next - lastY);

                    float t_right = static_cast<float>(n) / (num_points - 1);
                    edge_points_x_next_next[num_points + n] = normalizedX_next_next + t_right * (rightX_next_next - normalizedX_next_next);
                    edge_points_y_next_next[num_points + n] = normalizedY_next_next + t_right * (lastY - normalizedY_next_next);
                }

                for (int n = intersection_ID; n < 2 * num_points; ++n) {
                    for (int j = 0; j < num_points; ++j) {
                        if (std::abs(edge_points_x_next[n] - edge_points_x_next_next[j]) < 0.015f && std::abs(edge_points_y_next[n] - edge_points_y_next_next[j]) < 0.015f) {
                            intersection_next = true;
                            intersection_ID_next = j;
                        }
                    }

                    if (!intersection_next) {
                        glVertex2f(edge_points_x_next[n], edge_points_y_next[n]);
                    }

                    else {
                        break;
                    }
                }
                
                for (int n = intersection_ID_next; n < 2 * num_points; ++n) {
                    glVertex2f(edge_points_x_next[n], edge_points_y_next[n]);
                }

                i++;
            } while (intersection_next);
        
            glEnd();
        }
    }

    float finalX = xOffset + (maxRange / maxRange) * (1.0f - xOffset);
    glBegin(GL_LINE_STRIP);
    glVertex2f(lastX, lastY);
    glVertex2f(finalX, lastY);
    glEnd();
}

/*void CDrawAScope::drawDataPoints() {
    glColor3f(0.0f, 1.0f, 0.0f);  
    glLineWidth(1.0f);            

    float lastX = xOffset; 
    float lastY = yOffset + 0.5f * (1.0f - yOffset);  

    for (int i = 0; i < currentSize - 1; ++i) {
        float range = ranges[i];
        float amplitude = amplitudes[i];

        float range_next = ranges[i + 1];
        float amplitude_next = amplitudes[i + 1];

        if (range >= 0 && range <= maxRange) {
            float normalizedX = xOffset + (range / maxRange) * (1.0f - xOffset);
            float normalizedY = yOffset + ((amplitude / maxAmplitude) * (1.0f - yOffset) / 2.0f + 0.5f * (1.0f - yOffset));

            float normalizedX_next = xOffset + (range_next / maxRange) * (1.0f - xOffset);
            float normalizedY_next = yOffset + ((amplitude_next / maxAmplitude) * (1.0f - yOffset) / 2.0f + 0.5f * (1.0f - yOffset));

            float distBetweenGrids = (1.0f - xOffset) / gridXLines;
            detectedRange = distBetweenGrids / 8;

            float leftX = normalizedX - detectedRange;
            float rightX = normalizedX + detectedRange;

            float leftX_next = normalizedX_next - detectedRange;
            float rightX_next = normalizedX_next + detectedRange;

            int num_points = 40;

            float edge_points_x[80];       // 2 * num_points
            float edge_points_y[80];       // 2 * num_points

            float edge_points_x_next[80];  // 2 * num_points
            float edge_points_y_next[80];  // 2 * num_points

            glBegin(GL_LINE_STRIP);
            bool intersection = false;
            int intersection_ID = 0;


            for (int n = 0; n < 2 * num_points; ++n) {
                for (int j = 0; j < num_points; ++j) {
                    if (std::abs(edge_points_x[n] - edge_points_x_next[j]) < 0.015f && std::abs(edge_points_y[n] - edge_points_y_next[j]) < 0.015f) {
                        intersection = true;
                        intersection_ID = j;
                    }
                }

                if (!intersection) {
                    glVertex2f(edge_points_x[n], edge_points_y[n]);
                    i++;
                }
            }

            for (int n = intersection_ID; n < 2 * num_points; ++n) {
                glVertex2f(edge_points_x_next[n], edge_points_y_next[n]);
            }
            glEnd();
        }
    }

    float finalX = xOffset + (maxRange / maxRange) * (1.0f - xOffset);
    glBegin(GL_LINE_STRIP);
    glVertex2f(lastX, lastY);
    glVertex2f(finalX, lastY);
    glEnd();
}*/

void CDrawAScope::addDataPoint(float range[], float amplitude[], int size) {
    currentSize = 0;

    for (int i = 0; i < size; i++) {
        if (amplitude[i] > maxAmplitude) {
            maxAmplitude = amplitude[i];
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


CDrawAScope scope;