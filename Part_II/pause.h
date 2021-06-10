#pragma once
#include "M5Atom.h"

/* 
    A non-blocking delay solution that is more scalable than millis
*/

int updateDelay = 50; // milliseconds

int pauseTimer = 0; 
bool isPaused()
{
    delay(updateDelay);
    M5.update();
        
    if (pauseTimer > 0)
    {
        pauseTimer -= updateDelay;
        return true;
    }
    return false;
}
void resetPause(){ pauseTimer = 0; }
void pause(unsigned int pause){ pauseTimer += pause; }
