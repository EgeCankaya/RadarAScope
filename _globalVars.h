#pragma once
#ifndef _GLOBALVARS_H
#define _GLOBALVARS_H

class _Variables {
public:
	static _Variables* instance;
	_Variables();

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