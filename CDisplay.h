#include "_globalVars.h"
#include "CDrawAScope.h"


class CDisplay {
public:
    static void displayWrapper();
    void setCallbacks();
    static CDisplay& getInstance();
private:
    void display();
    void reshape(int w, int h);
    
    static void reshapeWrapper(int width, int height);
};


