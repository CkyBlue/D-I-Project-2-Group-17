
#include "M5Atom.h"

float accX = 0, accY = 0, accZ = 0;
//float gyroX = 0, gyroY = 0, gyroZ = 0;
float temp = 0;
float accXSum = 0, accYSum = 0, accZSum = 0;
float accXAverage = 0, accYAverage = 0, accZAverage = 0;
bool IMU6886Flag = true;
bool isDownwards = false;
bool positionChanged = false;
uint8_t i = 0;
uint8_t FSM = 0;
uint8_t level = 0;

char tilt = 'a';


//bool isUpwardsFunction ();
char isTilted ();


void setup()
{
    M5.begin(true, false, true);

    if (M5.IMU.Init() != 0)
        IMU6886Flag = false;
    else
        IMU6886Flag = true;
}

void loop()
{

    if (IMU6886Flag == true)
    {
        //M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
        M5.IMU.getAccelData(&accX, &accY, &accZ);
       // M5.IMU.getTempData(&temp);

      //  Serial.printf("%.2f,%.2f,%.2f o/s \r\n", gyroX, gyroY, gyroZ);
        Serial.printf("%.5f,%.5f,%.5f g\r\n", accX, accY, accZ);
        //Serial.printf("Temperature : %.2f C \r\n", temp);
    

      if (M5.Btn.wasPressed())
      {
        tilt = isTilted();
        if (tilt == 'u')
        {
            Serial.printf("upwards\n");
            isDownwards = false;
            while (isDownwards == false)
            {
              switch (level)
              {
                case 0:
                  Serial.printf("screen activated\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    tilt = isTilted();
                    delay(80);
                    if (tilt == 'r')
                    {
                      level++;
                      positionChanged = true;          
                    }
                    else if (tilt == 'l')
                    {
                      level = 5;
                      positionChanged = true;
                    }
                    else if (tilt == 'd')
                    {
                      isDownwards = true;
                      positionChanged = true;
                    }
                  }
                  break;
                case 1:
                  Serial.printf("mode 1\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    tilt = isTilted();
                    delay(80);
                    if (tilt == 'r')
                    {
                      level++;
                      positionChanged = true;          
                    }
                    else if (tilt == 'l')
                    {
                      level=5;
                      positionChanged = true;
                    }
                    else if (tilt == 'd')
                    {
                      level = 0;
                      isDownwards = true;
                      positionChanged = true;
                    }
                  }
                  break;
                case 2:
                  Serial.printf("mode 2\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    tilt = isTilted();
                    delay(80);
                    if (tilt == 'r')
                    {
                      level++;
                      positionChanged = true;          
                    }
                    else if (tilt == 'l')
                    {
                      level--;
                      positionChanged = true;
                    }
                    else if (tilt == 'd')
                    {
                      level = 0;
                      isDownwards = true;
                      positionChanged = true;
                    }
                  }
                  break;
                case 3:
                  Serial.printf("mode 3\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    tilt = isTilted();
                    delay(80);
                    if (tilt == 'r')
                    {
                      level++;
                      positionChanged = true;          
                    }
                    else if (tilt == 'l')
                    {
                      level--;
                      positionChanged = true;
                    }
                    else if (tilt == 'd')
                    {
                      level = 0;
                      isDownwards = true;
                      positionChanged = true;
                    }
                  }
                  break;
                case 4:
                  Serial.printf("mode 4\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    tilt = isTilted();
                    delay(80);
                    if (tilt == 'r')
                    {
                      level++;
                      positionChanged = true;          
                    }
                    else if (tilt == 'l')
                    {
                      level--;
                      positionChanged = true;
                    }
                    else if (tilt == 'd')
                    {
                      level = 0;
                      isDownwards = true;
                      positionChanged = true;
                    }
                  }
                  break;
                case 5:
                  Serial.printf("mode 5\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    tilt = isTilted();
                    delay(80);
                    if (tilt == 'r')
                    {
                      level=1;
                      positionChanged = true;          
                    }
                    else if (tilt == 'l')
                    {
                      level--;
                      positionChanged = true;
                    }
                    else if (tilt == 'd')
                    {
                      level = 0;
                      isDownwards = true;
                      positionChanged = true;
                    }
                  }
                  break;
                default:
                  break;
                
              }
            }
        }
      }
    }

    delay(100);
    M5.update();
}

char isTilted() // returns r if tilted right, l if tilted left, u if held upwards, d if held downwards
{
  for (i = 0; i<10; i++)
        {
          M5.IMU.getAccelData(&accX, &accY, &accZ);
          delay (50);
          //Serial.printf("%.2f,%.2f,%.2f g\r\n", accX, accY, accZ);
          accXSum += accX;
          accYSum += accY;
          accZSum += accZ;
          //Serial.printf("%.2f,%.2f,%.2f\n", accXSum, accYSum, accZSum);
        }
  accXAverage = accXSum/i;
  accYAverage = accYSum/i;
  accZAverage = accZSum/i;
  //Serial.printf("%.2f,%.2f,%.2f\n", accXAverage, accYAverage, accZAverage);
  if(abs(accXAverage) < 0.5 && abs(accYAverage) < 0.5 && accZAverage < 0) // device held upwards
  {
    accXSum = 0;
    accYSum = 0;
    accZSum = 0;
    return 'u';
  }
  else if (accXAverage > 0.5)
  {
    accXSum = 0;
    accYSum = 0;
    accZSum = 0;
    return 'r';
  }
  else if (accXAverage < -0.5)
  {
    accXSum = 0;
    accYSum = 0;
    accZSum = 0;
    return 'l';
  }
  else // device held downwards
  {
    accXSum = 0;
    accYSum = 0;
    accZSum = 0;
    return 'd';
  }
}
