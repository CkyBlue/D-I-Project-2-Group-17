#include "M5Atom.h"

float accX = 0, accY = 0, accZ = 0;
float gyroX = 0, gyroY = 0, gyroZ = 0;
float temp = 0;
bool IMU6886Flag = false;
//int test_range=10;
float accel_array[10];//array to check for acceleration
int test_range=10;
int maxval=0;
int minval=0;
int current_size;

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
        M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        M5.IMU.getTempData(&temp);

        //Serial.printf("%.2f,%.2f,%.2f o/s \r\n", gyroX, gyroY, gyroZ);
        Serial.printf("%.2f mg\r\n", accX * 1000);
        current_size=(sizeof(accel_array));
        if (current_size<test_range)//if current array size is less than 10, append to array
        {
          accel_array[current_size]=accX;
        }

        if (current_size=test_range)// if array is full, compare the 10 values and see if they are significantly different from eahc other or not
        {
          //maxvalue
          maxval=abs(accel_array[0]);
          int i=0;
          for (i=0;i<test_range;i++)
          {
            if (abs(accel_array[i])>abs(maxval))
            {
              maxval=abs(accel_array[i]);
            }
          }

          //minvalue
          minval=abs(accel_array[0]);
          int j=0;
          for (j=0;j<test_range;j++)
          {
            if (abs(accel_array[j])<abs(minval))
            {
              minval=abs(accel_array[j]);
            }
          }
          Serial.print("minval");
          Serial.print(minval);
          Serial.print("maxval");
          Serial.print(maxval);
          
          if (abs(maxval-minval)>500)//if they are signifcantly different, print' break is on'
          {
            Serial.printf("BREAKS ON!\n");
            current_size=0;//empty that array adn start again
          }
                  
                  
          
        }
        
        //Serial.printf("Temperature : %.2f C \r\n", temp);
    }
    delay(500);
    M5.update();
}
