#pragma once
#ifndef RADARASCOPE_H
#define RADARASCOPE_H

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

class DLL_EXPORT CRadarAScopeDisplay {
public:
    //If used with other freeGLUT init function(s) the freeGLUT wont work therefore the init function should be used once.
    void initAScope();
    
    void run();
    
    void endAScope();
    
    void addEnemy(float ranges[], float amplitudes[], int size);
};

#endif // RADARASCOPE_H
