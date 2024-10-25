#include "_globalVars.h"

_Variables* _Variables::instance = nullptr;

_Variables::_Variables() 
	: rangeShift(0), maxRange(300.0f), initialAmplitude(2.0f), 
	maxAmplitude(2.0f), detectedRange(0.0f), xOffset(0.1f), yOffset(0.1f),
	gridXLines(10), gridYLines(20), xLabelInterval(50.0f), yLabelInterval(0.1f){}

_Variables* _Variables::getInstance() {
	if (!instance)
		instance = new _Variables();
	return instance;
}
