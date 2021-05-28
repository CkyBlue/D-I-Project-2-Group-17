#include "M5Atom.h"

void setup()
{
    M5.begin(true, true, true);
    delay(10);
}

uint8_t state = 0;
// 0 - OFF, 1 - Manual Red Strobe, 2 - Manual White Strobe, 3 - Automatic Red Strobe, 4 - Automatic White Strobe

bool isBraking(){}

void strobe(CRGB color){}

void loop()
{
    if (M5.Btn.wasPressed())
    {
        switch (state)
        {
        case 0: // OFF

            break;
        case 1: // Manual Red Strobe

            break;
        case 2: // Manual White Strobe

            break;
        case 3: // Automatic Red Strobe

            break;
        case 4: // Automatic White Strobe

            break;
        default:
            break;
        }


        state++;
        if (state >= 5){ state = 0; }
    }

    delay(50);
    M5.update();
}
