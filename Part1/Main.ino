#include "M5Atom.h"
void setup()
{
    M5.begin(true, false, true);
}
void Strobe(CRGB color);


int FSM = 1;
void loop()
{
    if (M5.Btn.wasPressed())
    {
        switch (FSM)
        {
        case 0:
            //OFF STATE
            break;
        case 1:
            //STATE 1 Manual Rear strobe (RED)
            break;
        case 2:
            //STATE 2  Manual Front strobe (WHITE)
            break;
        case 3:
            //STATE 3 Automatic Rear Mode Rear (RED)
            break;
        case 4:
            //Automatic Front Mode Rear (WHITE)
            break;
        default:
            break;
        }
        FSM++;
        if(FSM>=5) FSM=0;
    }
}

strobe(CRGB color) {

    int strobeDelay = 500;
    int updateDelay = 50;

    int strobeSteps = strobeDelay/updateDelay;
    uint8_t strobeCount = 0;
    bool strobeFlag = false;

    strobeCount++;
    if (strobeCount >= strobeSteps){ strobeCount = 0; strobeFlag = !strobeFlag;}
    if (strobeFlag){
       M5.dis.fillpix(0xf00000);
    } else { 
        M5.dis.drawpix(1, 0x000000); 
    }
}