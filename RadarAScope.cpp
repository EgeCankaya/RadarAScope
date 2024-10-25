#include "RadarAScope.h"
#include <GL/freeglut.h>
#include "CDisplay.h"
#include "CDrawAScope.h"

CDisplay& _disp = CDisplay::getInstance();
CDrawAScope& _scope = CDrawAScope::getInstance();

void CRadarAScopeDisplay::run() {
    int argc = 1;
    char* argv[1] = { (char*)"RadarDisplay" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Radar A-Scope");

    _disp.setCallbacks();
    _scope.setCallbacks();
}

void CRadarAScopeDisplay::endAScope() {
    glutMainLoop();
}

void CRadarAScopeDisplay::addEntry(float ranges[], float amplitudes[], int size)
{   
    _scope.addDataPoint(ranges, amplitudes, size);
    glutPostRedisplay();
}
