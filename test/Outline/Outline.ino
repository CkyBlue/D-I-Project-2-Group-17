#include "M5Atom.h"

unsigned long prevTime = 0;
unsigned long currentTime = 0;

void setup()
{
    M5.begin(true, true, true);
    delay(10);

    M5.IMU.Init();
    M5.dis.clear();
}

void fillPix(unsigned int color){
    for (int i = 0; i < 25; i++) M5.dis.drawpix(color);
}

uint8_t state = 0;
// 0 - OFF, 1 - Manual Red Strobe, 2 - Manual White Strobe, 3 - Automatic Red Strobe, 4 - Automatic White Strobe

bool isBraking(bool isLEDonFront, float accZ)
{
    accZ = isLEDonFront ? -accZ : accZ;
    return accZ < -0.1;
}

bool isAccelerating(bool isLEDonFront, float accZ) {
    accZ = isLEDonFront ? -accZ : accZ;
    return accZ > 0;
}

int strobeDelay = 500;
int updateDelay = 20 ;

bool strobeFlag = false;

float pitch, roll, yaw;
float rad = PI / 180;

float accX = 0, accY = 0, accZ = 0;
int fadeDelay = 500;

unsigned int accZFade = 0;

void strobe(unsigned int color)
{
    currentTime = millis();
    if (currentTime - prevTime > strobeDelay)
    {
        if (strobeFlag)
        {
            fillPix(color);
        }
        else
            M5.dis.clear();

        prevTime = millis();
    }
}

char *modes[] = {"Off", "Manual Red", "Manual White", "Automatic Red", "Automatic White"};

unsigned long fadeStart = 0;
unsigned int fadeTime = 0;

void loop()
{
    if (M5.Btn.wasPressed())
    {
        state++;
        if (state >= 5)
            state = 0;

        Serial.print("Changed State to " + modes[state]);

        if (state == 0)
            M5.dis.clear();
    }

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
    if (isAccelerating(false, accZ)){
        M5.dis.clear();
        Serial.printf("Accelerating\n");
    }  else if(isBraking(false, accZ) || fadeTime > 0 ){
            
            if(isBraking(false, accZ) {
                fadeTime = fadeDelay;
                fadeStart = millis();
            } else 
                fadeTime = fadeDelay - (millis() - fadeStart);
            
            Serial.printf("Fade Time Left: %.2f ms \n", fadeTime);
            setBuff(0xff, 0x00, 0x00);
    }
        break;
    }
    case 4: // Automatic White Strobe
    {
    if (isAccelerating(true, accZ)){
        M5.dis.clear();
        Serial.printf("Accelerating\n");
    }  else if(isBraking(true, accZ) || fadeTime > 0 ){
            
            if(isBraking(true, accZ) {
                fadeTime = fadeDelay;
                fadeStart = millis();
            } else 
                fadeTime = fadeDelay - (millis() - fadeStart);
            
            Serial.printf("Fade Time Left: %.2f ms \n", fadeTime);
            setBuff(0xff, 0xff, 0xff);
    }
        break;
    }
    default:
        break;
    }

    delay(updateDelay);
    M5.update();
}
