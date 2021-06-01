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
    M5.update();
    delay(10);
  
    if (pauseTimer > 0)
    {
        pauseTimer -= 10;
        return true;
    }
    return false;
}
void resetPause(){ pauseTimer = 0; }
void pause(unsigned int delay){ pauseTimer += delay; }

bool strobeFlag = true;

void strobe(unsigned int color){
    strobeFlag = !strobeFlag;
    if (strobeFlag){
        M5.dis.drawpix(12, color);
    } else {
        M5.dis.drawpix(12, 0x000000);
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
        if (state > 1)
            state = 0;

        Serial.printf("Mode %.2f \n", state);
        
         M5.update();
         delay(20);
        return;
    }

    // Delay Implementation
    if (isPaused()) return;

    switch (state){
        case 0: {
          Serial.print("Red\n");
            strobe(0x00ff00); break;
        }
        case 1: {
          Serial.print("White\n");
            strobe(0xffffff); break;
        }
    }
    
    M5.update();
    delay(10);
}
