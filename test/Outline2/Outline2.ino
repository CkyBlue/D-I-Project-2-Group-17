#include "M5Atom.h"

void setup()
{
    M5.begin(true, true, true);
    delay(10);

    M5.IMU.Init();
    M5.dis.clear();
}

void fillPix(unsigned int color)
{
    for (int i = 0; i < 25; i++)
        M5.dis.drawpix(i, color);
}

uint8_t state = 0;
// 0 - OFF, 1 - Manual Red Strobe, 2 - Manual White Strobe, 3 - Automatic Red Strobe, 4 - Automatic White Strobe

bool isBraking(bool isLEDonFront, float accZ)
{
    accZ = isLEDonFront ? -accZ : accZ;
    return accZ < -0.04;
}

bool isAccelerating(bool isLEDonFront, float accZ)
{
    accZ = isLEDonFront ? -accZ : accZ;
    return accZ > 0.04;
}

int strobeDelay = 500;

bool strobeFlag = false;

float pitch, roll, yaw;
float rad = PI / 180;

float accX = 0, accY = 0, accZ = 0;
int fadeDelay = 500;

unsigned int accZFade = 0;

int pauseTimer = 0;
int wasDecelerating = true;

bool isPaused()
{
    delay(10);
    M5.update();

    if (pauseTimer > 0)
    {
        pauseTimer -= 10;
        return true;
    }
    return false;
}
void resetPause() { pauseTimer = 0; }
void pause(unsigned int delay) { pauseTimer += delay; }

void strobe(unsigned int color)
{
    strobeFlag = !strobeFlag;
    if (strobeFlag)
    {
        fillPix(color);
    }
    else
    {
        fillPix(0x000000);
    }
    pause(1000);
}

char *modes[] = {"Off", "Manual Red", "Manual White", "Automatic Red", "Automatic White"};

unsigned long fadeStart = 0, fadeTime = 0;

void loop()
{
    if (M5.Btn.wasPressed())
    {
        state++;
        if (state >= 5)
            state = 0;

        Serial.printf("Mode %.2f \n", state);

        if (state == 0)
            M5.dis.clear();

        resetPause();
        
        delay(20);
        M5.update();
        
        return;
    }

    if (isPaused())
        return;

    M5.IMU.getAhrsData(&pitch, &roll, &yaw);
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    accZ -= cos(roll * rad);

    switch (state)
    {

    case 1: // Manual Red Strobe
    {
        strobe(0x00ff00);
        break;
    }
    case 2: // Manual White Strobe
    {
        strobe(0xffffff);
        break;
    }
    case 3: // Automatic Red Strobe
    {
        if (isAccelerating(false, accZ))
        {
            fadeTime = 0;
            M5.dis.clear();
            
            if (wasDecelerating) Serial.printf("Accelerating\n");
            wasDecelerating = false;
        }
        else if (isBraking(false, accZ) || fadeTime > 0)
        {         
            if (isBraking(false, accZ)) fadeTime = 500;
            else fadeTime -= 20;
            
            fillPix(0xffffff);
            if (!wasDecelerating) Serial.printf("Decelerating\n");
            wasDecelerating = true;
            
        } else {
            strobe(0x00ff00);
        }
        break;
    }
    case 4: // Automatic White Strobe
    {
       if (isAccelerating(true, accZ))
        {
            M5.dis.clear();
            Serial.printf("Accelerating\n");
        }
        else if (isBraking(true, accZ))
        {         
            fillPix(0x00ff00);
            delay(500);
        } else {
            strobe(0xffffff);
        }
        break;
    }
    default:
        break;
    }

    delay(20);
    M5.update();
}
