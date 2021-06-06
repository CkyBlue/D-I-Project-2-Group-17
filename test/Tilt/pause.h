#pragma once
#include "M5Atom.h"

int pauseTimer = 0; 
bool isPaused()
{
    if (pauseTimer > 0)
    {
        delay(10);
        M5.update();

        pauseTimer -= 10;
        return true;
    }
    return false;
}
void resetPause(){ pauseTimer = 0; }
void pause(unsigned int pause){ pauseTimer += pause; }
