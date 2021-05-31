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

int updateDelay = 50;

int state = 0;
int newState = 0;
int constraint = 20;

bool isEqual(int val, int goal){return val >= goal - constraint && val <= goal + constraint;}

void loop()
{
    M5.dis.clear();
    
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);
    //Serial.printf("%.2f, %.2f \n", pitch, roll);
  
    if (isEqual(pitch, 0) && isEqual(abs(roll), 180))
      newState = 0;
    else if (isEqual(pitch, 90))
       newState = 1;
    else if (isEqual(pitch, -90))
       newState = 2;
    else if (isEqual(roll, 90))
       newState = 3;
    else if (isEqual(roll, -90))
       newState = 4;
    else if (isEqual(abs(pitch), 0) && isEqual(abs(roll), 0))
       newState = 5;
      
    if (newState != state){
      state = newState;
      switch (state){
        case 0:
         {Serial.print("Mode Facing Up\n"); break;}
         case 1:
         {Serial.print("Mode Left\n"); break;}
         case 2:
         {Serial.print("Mode Right\n"); break;}
         case 3:
         {Serial.print("Mode Front\n"); break;}
         case 4:
         {Serial.print("Mode Back\n"); break;}
         case 5:
         {Serial.print("Mode Upside Down\n"); break;}
        }
     }
//    setBuff(0xff, 0x00, 0x00);
//    M5.dis.displaybuff(DisBuff);
    
    delay(updateDelay);
    M5.update();
}
