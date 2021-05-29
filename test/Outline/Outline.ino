#include "M5Atom.h"

void setup()
{
    M5.begin(true, true, true);
    delay(10);
}

uint8_t DisBuff[2 + 5 * 5 * 3];

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
    }
}

uint8_t state = 0;
// 0 - OFF, 1 - Manual Red Strobe, 2 - Manual White Strobe, 3 - Automatic Red Strobe, 4 - Automatic White Strobe

//bool isBraking(){}

int strobeDelay = 500;
int updateDelay = 50;

int strobeSteps = strobeDelay / updateDelay;
uint8_t strobeCount = 0;
bool strobeFlag = false;
void strobe(uint8_t r, uint8_t g, uint8_t b)
{
    strobeCount++;
    if (strobeCount >= strobeSteps)
    {
        strobeCount = 0;
        strobeFlag = !strobeFlag;
        if (strobeFlag)
        {
            setBuff(r, g, b);
            M5.dis.displaybuff(DisBuff);
        }
        else
        {
            M5.dis.clear();
        }
    }
}

//void strobe(CRGB color){}

void loop()
{
    if (M5.Btn.wasPressed())
    {
        switch (state)
        {
        case 0: // OFF
        {
            Serial.printf("Mode is off\n");
            M5.dis.clear();
            break;
        }
        case 1: // Manual Red Strobe
        {
            Serial.printf("Manual Red mode\n");
            strobe(255, 0, 0);
            break;
        }
        case 2: // Manual White Strobe
        {
            Serial.printf("Manual White mode\n");
            strobe(255, 255, 255);
            break;
        }
        case 3: // Automatic Red Strobe
        {
            Serial.printf("Automatic Red mode\n");
            break;
        }
        case 4: // Automatic White Strobe
        {
            Serial.printf("Automatic White mode\n");
            break;
        }
        default:
            break;
        }

        state++;
        if (state >= 5)
        {
            state = 0;
        }
    }

    delay(updateDelay);
    M5.update();
}
