#ifndef _GLOBALVARS_H
#define _GLOBALVARS_H
#include <vector>

extern std::vector<std::pair<float, float>> dataPoints;

extern int rangeShift;
extern float maxRange;
extern float maxAmplitude;
extern float initialAmplitude;
extern float detectedRange;

extern float xOffset;
extern float yOffset;

extern int gridXLines;
extern int gridYLines;

extern float xLabelInterval;
extern float yLabelInterval;

#endif // _GLOBALVARS_H