#include "M5Atom.h"

float accX = 0, accY = 0, accZ = 0;
float gyroX = 0, gyroY = 0, gyroZ = 0;
float temp = 0;
float accXSum = 0, accYSum = 0, accZSum = 0;
float accXAverage = 0, accYAverage = 0, accZAverage = 0;
bool IMU6886Flag = false;
bool isUpwards = false;
uint8_t i = 0;

bool isUpwardsFunction ();



void setup()
{
    M5.begin(true, false, true);

    if (M5.IMU.Init() != 0)
        IMU6886Flag = false;
    else
        IMU6886Flag = true;
}

uint8_t FSM = 0;

void loop()
{
  if (M5.Btn.wasPressed())
  {
    isUpwards = isUpwardsFunction();
    if (isUpwards == true)
    {
      Serial.printf("upwards");
    }
    else
    {
      Serial.printf("not upwards");
    }

  }  
   
delay(500);
M5.update();
}

bool isUpwardsFunction ()
{
  for (i = 0; i<10; i++)
        {
          M5.IMU.getAccelData(&accX, &accY, &accZ);
          //Serial.printf("%.2f,%.2f,%.2f mg\r\n", accX * 1000, accY * 1000, accZ * 1000);
          accXSum += accX*1000;
          accYSum += accY*1000;
          accZSum += accZ*1000;
          //Serial.printf("%.2f,%.2f,%.2f\n", accXSum, accYSum, accZSum);
        }
  accXAverage = accXSum/i;
  accYAverage = accYSum/i;
  accZAverage = accZSum/i;
  //Serial.printf("%.2f,%.2f,%.2f\n", accXAverage, accYAverage, accZAverage);
  if(accXAverage > -600 && accXAverage < 600 && accYAverage > -600 && accYAverage < 600 && accZAverage < 0) 
  {
    return true;
  }
  else
  {
    return false;
  }

}
