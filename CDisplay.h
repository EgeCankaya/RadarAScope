#include "_globalVars.h"
#include "CDrawAScope.h"


class CDisplay {
public:
    static void displayWrapper();
    void setCallbacks();
private:
    void display();
    void reshape(int w, int h);
    static CDisplay& getInstance();
    static void reshapeWrapper(int width, int height);
};

extern CDisplay disp;

