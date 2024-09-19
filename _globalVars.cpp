#include "_globalVars.h"

std::vector<std::pair<float, float>> dataPoints;

int rangeShift = 0; 

float maxRange = 300.0f;

float initialAmplitude = 2.0f; 
float maxAmplitude = initialAmplitude;  
float detectedRange = 0.0f;

float xOffset = 0.1f;
float yOffset = 0.1f;

int gridXLines = 10;
int gridYLines = 20;

float xLabelInterval = 50.0f;
float yLabelInterval = 0.1f;
