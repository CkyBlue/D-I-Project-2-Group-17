#include "M5Atom.h"

CRGB led(0, 0, 0);
double pitch, roll;

float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float temp = 0;
void setup()
{
    M5.begin(true, true, true);
    M5.IMU.Init();
}

void loop()
{
    if (M5.Btn.wasPressed())
    {    Serial.printf("---------- \n");    }
  
    
    M5.IMU.getAttitude(&pitch, &roll);
    M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
    M5.IMU.getAccelData(&accX,&accY,&accZ);
    M5.IMU.getTempData(&temp);
    
    Serial.printf("%.2f, %.2f      ", pitch, roll);
    Serial.printf("%.2f   %.2f   %.2f      ", gyroX, gyroY, gyroZ);
    Serial.printf("%.2f   %.2f   %.2f      ",accX * 1000,accY * 1000, accZ * 1000);
    Serial.printf("Temperature : %.2f C \n", temp);

    //M5.dis.fillpix(led);
    delay(50);
    M5.update();
;
}
