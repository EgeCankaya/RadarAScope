#include "CDisplay.h"
#include "_globalVars.h"
#include <GL/freeglut.h>

CDrawAScope& scope = CDrawAScope::getInstance();

void CDisplay::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glOrtho(0.0f, 1.1f, 0.0f, 1.1f, -1.0f, 1.0f);

    setCallbacks();
    scope.setCallbacks();

    glutSwapBuffers();
}

void CDisplay::reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = (float)w / (float)h;
    if (aspectRatio > 1.0f) {
        glOrtho(-1.0f, 1.0f * aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
    }
    else {
        glOrtho(-1.0f, 1.0f, -1.0f, 1.0f / aspectRatio, -1.0f, 1.0f);
    }
    glMatrixMode(GL_MODELVIEW);
}

CDisplay& CDisplay::getInstance() {
    static CDisplay instance;
    return instance;
}

void CDisplay::displayWrapper() {
    getInstance().display();
}

void CDisplay::reshapeWrapper(int width, int height) {
    getInstance().reshape(width, height);
}

void CDisplay::setCallbacks() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);

    glutDisplayFunc(displayWrapper);
    glutReshapeFunc(reshapeWrapper);
}

