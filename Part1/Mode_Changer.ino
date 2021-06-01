#include <M5Atom.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <string>
#include <iostream>
#define PIN 27
#define array_size 5
#define LED_brightness 85
#define temp_array_size 24
using namespace std;
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
unsigned long millisBetweenTempUpdate = 86400000;
unsigned long millisOfLastButtonUpdate = 0;
unsigned long millisBetweenButtonUpdate = 50;
unsigned long millisOfLastTiltUpdate = 0;
unsigned long millisBetweenTiltUpdate = 400;
unsigned long currentMillis = 0;
class SensorData
{
private:
    float acZ[array_size];               //aceleration Z for past 5 Timestamps
    float temp[temp_array_size] = {-50}; //temperature sampled every hour
    float acZbef;
    int index;
    int i;
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
    float pitch, roll, yaw;
    float Activetemperature;
    float Average24;

public:
    SensorData()
    {
        accX = 0;
        accY = 0;
        accZ = 0;
        gyroX = 0;
        gyroY = 0;
        gyroZ = 0;
        Activetemperature = 0;
        Average24 = 0;
        i = 0;
        index = 0;
    }
    void update()
    {
        M5.IMU.getAhrsData(&pitch, &roll, &yaw);
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        //add to temperature array here ¬Rami
        if (i >= array_size)
        {
            acZbef = acZ[array_size - 1];
            i = 0;
        }
        else
        {
            acZ[i] = accZ;
            i++;
        }
    }
    void fetchAcc()
    {
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        if (i >= array_size)
        {
            acZbef = acZ[array_size - 1];
            i = 0;
        }
        else
        {
            acZ[i] = accZ;
            i++;
        }
    }
    void fetchAngles()
    {
        M5.IMU.getAhrsData(&pitch, &roll, &yaw);
    }
    float get24Average()
    {
        return Average24;
    }
    void addtotemparr(float temperature)
    {
        if (index >= temp_array_size)
        {
            index = 0;
        }
        temp[index] = temperature;
        index++;
    }
    void setAverage()
    {
        float sum;
        int z = 0;
        for (int i = 0; i < temp_array_size; i++)
        {
            if (temp[i] != -50)
            {
                sum += temp[i];
                z++;
            }
        }
        Average24 = sum / (float)z;
    }
    float getActiveTemp()
    {
        M5.IMU.getTempData(&Activetemperature);
        return Activetemperature;
    }
    bool isTap()
    { //check if tapping
        if (i == 0)
        { //use before
            if (abs(acZ[i] - acZbef) / abs(acZbef) > 0.1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (abs(acZ[i] - acZ[i - 1]) / abs(acZ[i - 1]) > 0.1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    bool isUp() //checks if vertically upwards
    {
        if ((abs(roll)) < 1 && (abs(pitch)) < 1 && (abs(yaw) < 1))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    char isTilted() // returns r if tilted right, l if tilted left, u if held upwards, d if held downwards
    {
        if (abs(accX) < 0.6 && abs(accY) < 0.6 && accZ < 0) // device held upwards
            return 'u';
        else if (accX > 0.4)
            return 'r';
        else if (accX < -0.4)
            return 'l';
        else // device held downwards
            return 'd';
    }
    void levelChangerSensor(uint8_t &level, bool &positionChanged, bool &isDownwards)
    {
        char orientation = isTilted();
        if (orientation == 'u')
        {
            positionChanged = false;
        }
        else if (orientation == 'r')
        {
            if (level == 4)
                level = 0;
            else
                level++;
            positionChanged = true;
        }
        else if (orientation == 'l')
        {
            if (level == 0 || level == 1)
                level = 4;
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
};
//scrolling text inspired by https://github.com/lukasmaximus89/M5Atom-Resources/blob/master/AtomScrollingText/AtomScrollingText.ino
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 5, PIN,
                                               NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
                                                   NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
                                               NEO_GRB + NEO_KHZ800);
class display
{
private:
    uint16_t color;
    bool canset;
    char unit;
    int x;
    float temperature;
    String output;

public:
    display()
    {
        //farenheight C and kelvin DEFAULT DEGREES C
        matrix.begin();
        matrix.setTextWrap(false);
        matrix.setBrightness(LED_brightness);
        matrix.setTextColor(color);
        color = matrix.Color(255, 255, 255);
        canset = true;
        temperature = 0;
        x = 0;
        unit = 'C';
        output = "";
    }
    void setTemp()
    {
    }
    void changeUnit()
    {
        if (unit == 'C')
        {
            temperature += 273.15;
            unit == 'K';
        }
        else if (unit == 'K')
        {
            temperature = (temperature - 273.15) * 9 / 5 + 32;
            unit == 'F';
        }
        else
        {
            temperature = (temperature - 32) * 5 / 9;
            unit == 'C';
        }
    }
    bool cansettemp()
    {
        if (canset)
        {
            canset != canset;
            return true;
        }
        else
        {
            false;
        }
    }
    void resetScreen(){
        x=0;
        matrix.clear();
    }
    void displayTemperature(float temp)
    {
        if (cansettemp())
        {
            temperature = temp;
            output = "";
            output.concat(temperature);
            Serial.print(output);
            output += unit;
        } else {
            resetScreen();
        }

        matrix.fillScreen(0);
        matrix.setCursor(x, 0);
        matrix.print(output);
        if (--x < -96)
        {
            x = matrix.width();
            matrix.setTextColor(color);
            canset = true;
        }
        matrix.show();
    }
};

SensorData tl;
display ds;

void update()
{
    currentMillis = millis();

    if (currentMillis - millisOfLastButtonUpdate > millisBetweenButtonUpdate)
    {
        M5.update();
        tl.fetchAcc();
        tilt = tl.isTilted();
        millisOfLastButtonUpdate = millis();
    }

    if (currentMillis - millisOfLastTempUpdate > millisBetweenTempUpdate)
    {
        //M5.IMU.getTempData(&temp);
        //Serial.printf("Temperature: %.2f °C \n", tl.getActiveTemp());
        tl.addtotemparr(tl.getActiveTemp()); //recalculate 24 hr average and update data
        millisOfLastTempUpdate = millis();
    }
}

void mode4()
{
}

void setup()
{
    M5.begin(true, false, true);
    if (M5.IMU.Init() != 0)
        IMU6886Flag = false;
    else
        IMU6886Flag = true;
    tl.fetchAcc();
    tl.addtotemparr(tl.getActiveTemp());
}
void loop()
{
    if (IMU6886Flag == true)
    {
        update();
        if (M5.Btn.wasPressed() /*|| tl.isTap()*/ && (tilt == 'u'))
        {
            Serial.printf("upwards\n");
            isDownwards = false;
            positionChanged = false;
            while (!isDownwards)
            {
                update();
                currentMillis = millis();
                if (currentMillis - millisOfLastTiltUpdate > millisBetweenTiltUpdate)
                {
                    tl.fetchAcc();
                    tilt = tl.isTilted();
                    millisOfLastTiltUpdate = millis();
                    switch (level)
                    {
                    case 0:
                        Serial.printf("screen activated\n");
                        //ds.displayTemperature(tl.getActiveTemp());//NO GO because its changing

                        tl.levelChangerSensor(level, positionChanged, isDownwards);
                        break;
                    case 1:
                        //24hr temp
                        Serial.printf("mode 2\n");

                        millisOfLastTiltUpdate = millis();
                        //ds.displayTemperature(tl.get24Average());
                        tl.levelChangerSensor(level, positionChanged, isDownwards);

                        break;
                    case 2:
                        Serial.printf("mode 3\n");

                        tl.levelChangerSensor(level, positionChanged, isDownwards);

                        break;
                    case 3:
                        Serial.printf("mode 4\n");

                        tl.levelChangerSensor(level, positionChanged, isDownwards);

                        break;
                    case 4:
                        Serial.printf("mode 5\n");

                        mode4();

                        tl.levelChangerSensor(level, positionChanged, isDownwards);

                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}
