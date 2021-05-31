#include "M5Atom.h"

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

void setup()
{
    M5.begin(true, true, true);
    M5.IMU.Init();

    delay(10);
}

float pitch, roll, yaw;
float rad = PI / 180;

float accX = 0, accY = 0, accZ = 0;
int updateDelay = 50;

int state = 0;
unsigned int accXFade = 0, accYFade = 0, accZFade = 0;

bool overDecelThreshold(float acc, int state)
{
    return (acc * 1000 < (-50 * (state + 1)));
}

bool overAccelThreshold(float acc, int state)
{
    return (acc * 1000 > (50 * (state + 1)));
}

void loop()
{
    M5.dis.clear();
    
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);

    if (M5.Btn.wasPressed())
    {
        state++;
        if (state >= 5)
            state = 0;
    }

    accZ -= cos(roll * rad);

    if (overDecelThreshold(accZ, state) || accZFade > 0)
    {
        if (overDecelThreshold(accZ, state))
            accZFade = 5;
        else
            accZFade--;

        setBuff(0x00, 0x00, 0x00);
        M5.dis.displaybuff(DisBuff);

        Serial.printf("Decelerating\n");
    }
    else if (overAccelThreshold(accZ, state))
    {
        M5.dis.clear();
        Serial.printf("Accelerating\n");
    }

    M5.dis.drawpix(state, 0xffffff);

    delay(updateDelay);
    M5.update();
}
