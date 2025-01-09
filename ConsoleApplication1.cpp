#include "RadarAScope.h"
#include <ctime>
#include <cstdlib>
#include <GL/freeglut.h>
#include <iostream>

float range[301], amplitude[301];
int size;

CRadarAScopeDisplay* aScope = nullptr;

void timerFunc(int value) {
    int randSize = rand() % 150;

    for (int i = 0; i < randSize; i++) {
        range[i] = rand() % 300;
        amplitude[i] = ((rand() % 100) / 10.0f) - 5.0f;
    }
    aScope->addEnemy(range, amplitude, randSize);
  

    glutTimerFunc(value, timerFunc, value);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    aScope = new CRadarAScopeDisplay();
    
    aScope->initAScope();
    aScope->run();
    glutTimerFunc(4000, timerFunc, 4000); 
    aScope->endAScope();
  
    delete aScope;

    return 0;
}
