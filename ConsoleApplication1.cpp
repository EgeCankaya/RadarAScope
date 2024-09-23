#include "AScope.h"
#include <ctime>
#include <cstdlib>
#include <GL/freeglut.h>

float range[301], amplitude[301];
int size;
CRadarAScope* aScope = nullptr; 

void timerFunc(int value) {
    int randSize = rand() % 30;
    
    for (int i = 0; i < randSize; i++) {
        range[i] = rand() % 300;
        amplitude[i] = (rand() % 50) / 10.0f;
    }
    aScope->addEntry(range, amplitude, randSize);

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
