#include "RadarAScope.h"
#include <GL/freeglut.h>
#include "CDisplay.h"
#include "CDrawAScope.h"
#include <vector>

CDisplay& _disp = CDisplay::getInstance();
CDrawAScope& _scope = CDrawAScope::getInstance();

void CRadarAScopeDisplay::initAScope() {
    int argc = 1;
    char* argv[1] = { (char*)"RadarDisplay" };
    glutInit(&argc, argv);
}

void CRadarAScopeDisplay::run() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Radar A-Scope");

    _disp.setCallbacks();
}

void CRadarAScopeDisplay::endAScope() {
    glutMainLoop();
}

std::vector<std::pair<float, float>> dataPoints;

void CRadarAScopeDisplay::addEnemy(float ranges[], float amplitudes[], int size) {   
    _scope.addDataPoint(ranges, amplitudes, size);
    glutPostRedisplay();
}
