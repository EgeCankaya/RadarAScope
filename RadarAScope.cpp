#include "RadarAScope.h"
#include <GL/freeglut.h>
#include "CDisplay.h"
#include "CDrawAScope.h"

void CRadarAScopeDisplay::run() {
    int argc = 1;
    char* argv[1] = { (char*)"RadarDisplay" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Radar A-Scope");

    disp.setCallbacks();
    scope.setCallbacks();
}

void CRadarAScopeDisplay::endAScope() {
    glutMainLoop();
}

void CRadarAScopeDisplay::addEntry(float range, float amplitude) {
    scope.addDataPoint(range, amplitude);
    glutPostRedisplay();
}
