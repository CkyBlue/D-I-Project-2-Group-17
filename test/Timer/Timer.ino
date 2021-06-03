#include "M5Atom.h"

void setup()
{
    M5.begin(true, true, true);
    M5.IMU.Init();

    delay(10);
}

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
void pause(unsigned int delay){ pauseTimer += delay; }

bool strobeFlag = true;

void fillPix(unsigned int color)
{
    for (int i = 0; i < 25; i++)
        M5.dis.drawpix(i, color);
}

void strobe(unsigned int color){
    strobeFlag = !strobeFlag;
    if (strobeFlag){
        fillPix(color);
    } else {
        fillPix(0x000000);
    }
    pause(1000);
}

int state = 0;

void loop()
{
    // Delay Immune Code
    if (M5.Btn.wasPressed()){
        resetPause();

        state++;
        if (state >= 2)
            state = 0;

        delay(10);
        M5.update();
        return;
    }

    // Delay Implementation
    if (isPaused()) return;

    switch (state){
        case 0: {
            strobe(0x00ff00); break;
        }
        case 1: {
            strobe(0xffffff); break;
        }
    }
    
    delay(10);
    M5.update();
}
