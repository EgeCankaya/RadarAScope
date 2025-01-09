#pragma once

class CDisplay {
public:
    void setCallbacks();
    static CDisplay& getInstance();
private:
    void display();
    static void displayWrapper();
    void reshape(int w, int h);
    static void reshapeWrapper(int width, int height);
};


