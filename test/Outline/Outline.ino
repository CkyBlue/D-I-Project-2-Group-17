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

void strobe(uint8_t r, uint8_t g, uint8_t b)
{
    currentTime = millis();
    if (currentTime - prevTime > strobeDelay)
    {
        if (strobeFlag)
        {
            setBuff(r, g, b);
            M5.dis.displaybuff(DisBuff);
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

        Serial.printf("Changed State to " + modes[state]);

        if (state == 0)
            M5.dis.clear();
    }

    accZ -= cos(roll * rad);

    switch (state)
    {

    case 1: // Manual Red Strobe
    {

        strobe(255, 0, 0);
        break;
    }
    case 2: // Manual White Strobe
    {
        strobe(255, 255, 255);
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
