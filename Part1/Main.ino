#include <Arduino.h>
#include <M5Atom.h>
#include <FastLED.h>
#include <math.h>
#include <thread>
#define percentchange 10 //the percentage change over 2 seconds for it to be braking
#define update_delay 50
#define strobe_delay 750
#define array_size 5
#define LED_brightness 85
using namespace std;
//accZ - cos(roll*rad)

//Initialize Variables
int FSM = 0;
float U, acc, percentacc;
int i;
double DEGREE_TO_RAD = 180 / PI;
bool x;
CRGB color;
class bike
{
    //this needs to contain the 3 x,y,and z acceleration values of the device, and a method to calculate MMA.
private:
    float accX, accY, accZ;
    float MMAX, MMAY, MMAZ;
    float pitch,roll,yaw;
    float MMAZB;
    float getMMA(float yesterday, float today, int N)
    {
        return (((N - 1) * yesterday + today) / N);
    } //returns Modified Moving Average at the next timestamp from acc array
    void setAcc()
    {
        M5.IMU.getAhrsData(&pitch,&roll,&yaw);
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        accX+=sin(pitch*DEGREE_TO_RAD);
        accY-=sin(roll*DEGREE_TO_RAD);
        accZ-=cos(roll*DEGREE_TO_RAD);
        //this is to cancel out the affect of gravitational acceleration when rotating the device
        
    } //retrieve acceleration data
public:
    bike()
    {
        accX = 0;
        accY = 0;
        accZ = 0;
        MMAX = 0;
        MMAY = 0;
        MMAZ = 0;
        MMAZB = 0;
    }
    float getAccX() { return accX; }
    float getAccY() { return accY; }
    float getAccZ() { return accZ; }
    float getMMAX() { return MMAX; }
    float getMMAY() { return MMAY; }
    float getMMAZ() { return MMAZ; }

    void setMMA(bike before)
    {
        setAcc();
        MMAX = getMMA(before.getMMAX(), accX, array_size);
        MMAY = getMMA(before.getMMAY(), accY, array_size);
        MMAZ = getMMA(before.getMMAZ(), accZ, array_size);
        MMAZB = before.getMMAZ();
    } //set the MMA for this current object

    bool isbraking(bool isFront)
    {
        //max a biker can pedal is 0.03g, so any moving average over 0.03 COULD be braking
        //to confirm, check if moving average of z-acceleration(+z points behind LEDs) is decreasing or increasing
        float mag = abs(sqrt(pow(MMAX, 2) + pow(MMAY, 2) + pow(MMAZ, 2)));
        Serial.printf("%.2f\n", mag);
        if (isFront)
        {
            if (mag > 0.03 && MMAZ >= MMAZB)
            {
                return true;
            }
            else
            {
                return false;
            }
        } else {
            if (mag > 0.03 && MMAZ <= MMAZB)
            {
                return true;
            }
            else
            {
                return false;
            }
        } 
    }
};

class strobe
{
private:
    CRGB color;
    int strobeDelay, updateDelay, strobeSteps;
    uint8_t strobeCount;
    bool strobeFlag;

public:
    strobe()
    {
        strobeCount = 0;
        strobeFlag = false;
        strobeDelay = strobe_delay;
        updateDelay = update_delay;
    }
    void strobeLight()
    {
        strobeCount++;
        if (strobeCount >= strobeSteps)
        {
            strobeCount = 0;
            strobeFlag = !strobeFlag;
        }
        if (strobeFlag)
        {
            M5.dis.fillpix(color);
        }
        else
        {
            M5.dis.clear();
        }
    }
    void resetstrobe()
    {
        strobe();
    }
    void setColor(CRGB color)
    {
        this->color = color;
    }
    CRGB getColor()
    {
        return color;
    }
};
bike MA[array_size];
strobe stro;
int fade = 0;
float getAvg(float *arr, int si)
{
    float sum = 0;
    for (int i = si; i < si; i++)
    {
        sum += arr[i];
    }
    return sum / (float)si;
}

void setup()
{
    M5.begin(true, true, true);
    M5.IMU.Init();
    M5.dis.setBrightness(LED_brightness);
}

void loop()
{
    switch (FSM)
    {
    case 0:
    {
        //OFF STATE
        M5.dis.clear();
    }
    break;
    case 1:
    {
        //STATE 1 Manual Rear strobe (RED)
        stro.setColor(CRGB::Green);
        stro.strobeLight();
    }
    break;
    case 2:
    {
        //STATE 2  Manual Front strobe (WHITE)
        stro.setColor(CRGB::White);
        stro.strobeLight();
    }
    break;
    case 3:
    {
        //STATE 3 Automatic Rear Mode Rear (RED)
        //LEDs are solid during a braking event. Return to strobe when riding
        stro.setColor(CRGB::Green);
        x = true;
        i = 0;
        while (x)
        {
            if (i == 0)
            {
                MA[i].setMMA(MA[0]); //fetch acceleration and set Moving average
            }
            else
            {
                MA[i].setMMA(MA[i - 1]);
            }
            //check for braking
            bool isbrake=MA[i].isbraking(false);
            if (isbrake || fade > 0)
            {
                if (isbrake) fade = 20;
                else {fade--;};

                M5.dis.fillpix(stro.getColor());
            }
            else
            {
                stro.strobeLight();
            }
            i++;
            if (M5.Btn.wasPressed())
            {
                break;
            }
            if (i >= array_size)
            {
                MA[0] = MA[array_size - 1];
                i = 0;
            }
            delay(update_delay);
            M5.update();
        }
    }
    break;
    case 4:
    {
        stro.setColor(CRGB::White);
        x = true;
        i = 0;
        while (x)
        {
            if (i == 0)
            {
                MA[i].setMMA(MA[0]); //fetch acceleration and set Moving average
            }
            else
            {
                MA[i].setMMA(MA[i - 1]);
            }
            //check for braking
            //check for braking
            bool isbrake=MA[i].isbraking(true);
            if (isbrake || fade > 0)
            {
                if (isbrake) fade = 20;
                else {fade--;};

                M5.dis.fillpix(stro.getColor());
            }
            else
            {
                stro.strobeLight();
            }
            i++;
            if (M5.Btn.wasPressed())
            {
                break;
            }
            if (i >= array_size)
            {
                MA[0] = MA[array_size - 1];
                i = 0;
            }
            delay(update_delay);
            M5.update();
        }
    }
    break;
    default:
        break;
    }
    if (M5.Btn.wasPressed())
    {
        FSM++;
        stro.resetstrobe();
        if (FSM >= 5)
            FSM = 0;
    }
    delay(update_delay);
    M5.update();
}
