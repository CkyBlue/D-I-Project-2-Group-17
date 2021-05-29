#include "M5Atom.h"

void setup()
{
    M5.begin(true, true, true);
    delay(10);
}

float accX = 0;
float accY = 0;
float accZ = 0;

int updateDelay = 50;

int state = -1;

int accXFade = 0;
int accYFade = 0;
int accZFade = 0;

void loop()
{
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    M5.dis.clear();

    if (M5.Btn.wasPressed())
    {state++;
        if (state >= 4)
        {
            state = -1;
        }
    }

    M5.dis.drawpix(state + 1, 0xffffff);

   Serial.printf("%.2f   %.2f   %.2f \n",accX * 1000,accY * 1000, accZ * 1000);

    if ((accX*1000 < -50 * (state + 1)) || accXFade > 0){
        if (accX*1000 < -50 * (state + 1)) accXFade = 10;
        else {accXFade--;}
        
        M5.dis.drawpix(5 + 1, 0xff0000);
        M5.dis.drawpix(5 + 2, 0xff0000);
        M5.dis.drawpix(5 + 3, 0xff0000);

        Serial.printf("Flash Red\n");
    }

    if ((accY*1000 < -50 * (state + 1)) || accYFade > 0){
        if (accY*1000 < -50 * (state + 1)) accYFade = 10;
        else {accYFade--;}
        M5.dis.drawpix(5*2 + 1, 0x00ff00);
        M5.dis.drawpix(5*2 + 2, 0x00ff00);
        M5.dis.drawpix(5*2 + 3, 0x00ff00);

        Serial.printf("Flash Green\n");
    }

    if ((accZ*1000 < -50 * (state + 1)) || accZFade > 0){
        if (accZ*1000 < -50 * (state + 1)) accZFade = 10;
        else {accZFade--;}
        M5.dis.drawpix(5*3 + 1, 0x0000ff);
        M5.dis.drawpix(5*3 + 2, 0x0000ff);
        M5.dis.drawpix(5*3 + 3, 0x0000ff);

        Serial.printf("Flash Blue\n");
    }

    delay(updateDelay);
    M5.update();
}
