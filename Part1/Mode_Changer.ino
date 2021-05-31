#include "M5Atom.h"

float accX = 0, accY = 0, accZ = 0;
float gyroX = 0, gyroY = 0, gyroZ = 0;
float temp = 0;
bool IMU6886Flag = true;
bool isDownwards = false;
bool positionChanged = false;
bool wasPressed = false;
uint8_t ctr = 0;
uint8_t FSM = 0;
uint8_t level = 0;
char tilt = 'a';
unsigned long millisOfLastTempUpdate = 0;
unsigned long millisBetweenTempUpdate = 2000;
unsigned long millisOfLastButtonUpdate = 0;
unsigned long millisBetweenButtonUpdate = 50;
unsigned long millisOfLastTiltUpdate = 0;
unsigned long millisBetweenTiltUpdate = 400;
unsigned long currentMillis = 0;

char isTilted ();
void levelChangerSensor (uint8_t& level, bool& positionChanged, bool& isDownwards);

void mode4(){

} 

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
           if (M5.Btn.wasPressed())
      {
        tilt = isTilted();
        if (tilt == 'u')
        {
          millisOfLastTiltUpdate = millis();
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
                    currentMillis = millis();
                    if (currentMillis - millisOfLastTiltUpdate > millisBetweenTiltUpdate)
                    {
                      tilt = isTilted();
                      millisOfLastTiltUpdate = millis();
                      levelChangerSensor (level, positionChanged, isDownwards);
                    }
                  }
                  break;
                case 1:
                  Serial.printf("mode 1\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    currentMillis = millis();
                    if (currentMillis - millisOfLastTiltUpdate > millisBetweenTiltUpdate)
                    {
                      tilt = isTilted();
                      millisOfLastTiltUpdate = millis();
                      levelChangerSensor (level, positionChanged, isDownwards);
                    }
                  }
                  break;
                case 2:
                  Serial.printf("mode 2\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    currentMillis = millis();
                    if (currentMillis - millisOfLastTiltUpdate > millisBetweenTiltUpdate)
                    {
                      tilt = isTilted();
                      millisOfLastTiltUpdate = millis();
                      levelChangerSensor (level, positionChanged, isDownwards);
                    }
                  }
                  break;
                case 3:
                  Serial.printf("mode 3\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    currentMillis = millis();
                    if (currentMillis - millisOfLastTiltUpdate > millisBetweenTiltUpdate)
                    {
                      tilt = isTilted();
                      millisOfLastTiltUpdate = millis();
                      levelChangerSensor (level, positionChanged, isDownwards);
                    }
                  }
                  break;
                case 4:
                  Serial.printf("mode 4\n");
                  mode4();
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    currentMillis = millis();
                    if (currentMillis - millisOfLastTiltUpdate > millisBetweenTiltUpdate)
                    {
                      tilt = isTilted();
                      millisOfLastTiltUpdate = millis();
                      levelChangerSensor (level, positionChanged, isDownwards);
                    }
                  }
                  break;
                case 5:
                  Serial.printf("mode 5\n");
                  positionChanged = false;
                  while (positionChanged == false)
                  {
                    currentMillis = millis();
                    if (currentMillis - millisOfLastTiltUpdate > millisBetweenTiltUpdate)
                    {
                      tilt = isTilted();
                      millisOfLastTiltUpdate = millis();
                      levelChangerSensor (level, positionChanged, isDownwards);
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
    currentMillis = millis();

    if (currentMillis - millisOfLastButtonUpdate > millisBetweenButtonUpdate)
    {
      M5.update();
      millisOfLastButtonUpdate = millis();
    }    

    if (currentMillis - millisOfLastTempUpdate > millisBetweenTempUpdate)
    {
      M5.IMU.getTempData(&temp);
      Serial.printf("Temperature: %.2f °C \n", temp);
      millisOfLastTempUpdate = millis();
    }
}


char isTilted() // returns r if tilted right, l if tilted left, u if held upwards, d if held downwards
{
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  if(abs(accX) < 0.6 && abs(accY) < 0.6 && accZ < 0) // device held upwards
    return 'u';
  else if (accX > 0.4)
    return 'r';
  else if (accX < -0.4)
    return 'l';
  else // device held downwards
    return 'd';
}


void levelChangerSensor (uint8_t& level, bool& positionChanged, bool& isDownwards)
{
  char orientation = isTilted();
  if (orientation == 'r')
  {
    if (level == 5)
      level = 1;
    else
      level++;  
    positionChanged = true; 
  }
  else if (orientation == 'l')
  {
    if (level == 0 || level == 1)
      level = 5;
    else
      level--;
    positionChanged = true;
  }
  else if (orientation == 'd')
  {
    level = 0;
    Serial.printf("tilted downwards\n");
    isDownwards = true;
    positionChanged = true;
  }
}
