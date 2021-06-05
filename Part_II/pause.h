#pragma once
#include "M5Atom.h"

int pauseTimer = 0; 
bool isPaused()
{
    delay(50);
    M5.update();
        
    if (pauseTimer > 0)
    {
        pauseTimer -= 50;
        return true;
    }
    return false;
}
void resetPause(){ pauseTimer = 0; }
void pause(unsigned int pause){ pauseTimer += pause; }
