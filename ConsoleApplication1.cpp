#include "AScope.h"
#include <ctime>
#include <cstdlib>
#include <GL/freeglut.h>

float km = 0.0f, v = 0.0f;
CRadarAScope* aScope = nullptr; 

void timerFunc(int value) {
    km = rand() % 300;
    v = (rand() % 50) / 10.0f;
    
    aScope->addEntry(km, v);
    glutTimerFunc(value, timerFunc, value);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    CRadarAScopeDisplay display;
    aScope = new CRadarAScope(&display);  

    aScope->run();
    glutTimerFunc(4000, timerFunc, 4000); 
    aScope->endAScope();
  
    delete aScope;

    return 0;
}
