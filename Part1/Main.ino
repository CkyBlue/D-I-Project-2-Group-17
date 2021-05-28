#include <M5Atom.h>
//#include <Arduino.h>
//#include <FastLED.h>
void setup()
{
    M5.begin(true, false, true);
}
void Strobe(CRGB color);
void CalibrateSensor();//callibrate sensor to 
int FSM = 1;
void loop()
{
    switch (FSM)
    {
    case 0:
        //OFF STATE
        break;
    case 1:
        //STATE 1 Manual Rear strobe (RED)
        strobe(CRGB::red);
        break;
    case 2:
        //STATE 2  Manual Front strobe (WHITE)
        strobe((CRGB)CHSV(255, 255, 255));
        break;
    case 3:
        //STATE 3 Automatic Rear Mode Rear (RED)
        //strobe(CRGB::white);
        bool x = true;
        do
        {
            //check for braking

            //
        } while (x);
        break;
    case 4:
        //Automatic Front Mode Rear (WHITE)
        //strobe();
        break;
    default:
        break;
    }
    if (M5.Btn.wasPressed())
    {
        FSM++;
        if (FSM >= 5) FSM = 0;
    }
}

strobe(CRGB color)
{

    int strobeDelay = 500;
    int updateDelay = 50;

    int strobeSteps = strobeDelay / updateDelay;
    uint8_t strobeCount = 0;
    bool strobeFlag = false;

    strobeCount++;
    if (strobeCount >= strobeSteps)
    {
        strobeCount = 0;
        strobeFlag = !strobeFlag;
    }
    if (strobeFlag)
    {
        M5.dis.fillpix(0xf00000);
    }
    else
    {
        M5.dis.drawpix(1, 0x000000);
    }
}