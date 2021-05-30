#include "M5Atom.h"

void setup()
{
    M5.begin(true, true, true);
    M5.IMU.Init();
    delay(10);
}

float accX = 0;
float accY = 0;
float accZ = 0;

int updateDelay = 50;

int state = 0;

int accXFade = 0;
int accYFade = 0;
int accZFade = 0;

bool overDecelThreshold(float acc, int state){
 return (acc * 1000 < (-50 * (state + 1)));
}

bool overAccelThreshold(float acc, int state){
 return (acc * 1000 > (50 * (state + 1)));
}

void loop()
{
    M5.dis.clear();
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    

    if (M5.Btn.wasPressed())
    {state++;
        if (state >= 5)
        {
            state = 0;
        }
    }

    M5.dis.drawpix(state, 0xffffff);

   Serial.printf("%.2f   %.2f   %.2f \n",accX * 1000,accY * 1000, accZ * 1000);

/*     if (overDecelThreshold(accX, state) || accXFade > 0){
        if (overDecelThreshold(accX, state)) accXFade = 5;
        else {accXFade--;}
        
        for (int i = 1; i <= 3; i++){
            M5.dis.drawpix(5 + i, 0xff0000);
        }

        Serial.printf("Flash Inside Red\n");
    } else if (overAccelThreshold(accX, state)){
        M5.dis.drawpix(5, 0xff0000);
        M5.dis.drawpix(5 + 4, 0xff0000);
        Serial.printf("Flash Outside Red\n");
    } */

    if (overDecelThreshold(accY, state) || accYFade > 0){
        if (overDecelThreshold(accY, state)) accYFade = 5;
        else {accYFade--;}
        
        for (int i = 1; i <= 3; i++){
            M5.dis.drawpix(5*2 + i, 0x00ff00);
        }

        Serial.printf("Flash Inside Green\n");
    } else if (overAccelThreshold(accY, state)){
        M5.dis.drawpix(5*2, 0x00ff00);
        M5.dis.drawpix(5*2 + 4, 0x00ff00);
        Serial.printf("Flash Outside Green\n");
    }

/*     if (overDecelThreshold(accZ, state) || accZFade > 0){
        if (overDecelThreshold(accZ, state)) accZFade = 5;
        else {accZFade--;}
        
        for (int i = 1; i <= 3; i++){
            M5.dis.drawpix(5*3 + i, 0x0000ff);
        }

        Serial.printf("Flash Inside Blue\n");
    } else if (overAccelThreshold(accZ, state)){
        M5.dis.drawpix(5*3, 0x0000ff);
        M5.dis.drawpix(5*3 + 4, 0x0000ff);
        Serial.printf("Flash Outside Blue\n");
    } */

    delay(updateDelay);
    M5.update();
}
