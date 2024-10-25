#ifndef _GLOBALVARS_H
#define _GLOBALVARS_H
#include <vector>

class _Variables {
public:
	static _Variables* instance;
	_Variables();

	std::vector<std::pair<float, float>> dataPoints;
	int rangeShift;
	float maxRange;
	float maxAmplitude;
	float initialAmplitude;
	float detectedRange;
	
	float xOffset;
	float yOffset;
	
	int gridXLines;
	int gridYLines;
	
	float xLabelInterval;
	float yLabelInterval;

	static _Variables* getInstance();
};


#endif // _GLOBALVARS_H