#include <Arduino.h>
#include <M5Atom.h>
#include <FastLED.h>
#include <math.h>
#include <thread>
#define percentchange 10 //the percentage change over 2 seconds for it to be braking
#define update_delay 50
#define strobe_delay 500
#define array_size 5
using namespace std;

//Initialize Variables
int FSM = 1;
float U, acc, percentacc;
bool firstTime;                     //the first time acceleration data is retrieved
float accX = 0, accY = 0, accZ = 0; //acceleration values of the device
bike MA[array_size];//bike objects over 5 timestamps
CRGB color;
strobe stro;
class bike
{
    //this needs to contain the 3 x,y,and z acceleration values of the device, and a method to calculate MMA.
private:
    float accX, accY, accZ;
    float MMAX, MMAY, MMAZ;
    float getMMA(float yesterday, float today, int N)
    {
        return (((N - 1) * yesterday + today) / N);
    } //returns Modified Moving Average at the next timestamp from acc array
    void setAcc()
    {
        M5.IMU.getAccelData(&accX, &accY, &accZ);
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
    } //set the MMA for this current object

    bool isbraking()
    {
        //max a biker can pedal is 0.4g, so any moving average over 0.4 is braking
        if(MMAX>0.4||MMAY>0.4||MMAZ>0.4){
            return true;
        } else {
            return false;
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
    M5.begin(true, false, true);
    firstTime = true;
    percentacc = update_delay / 1000 * percentchange / 2;
}

void loop()
{
    switch (FSM)
    {
    case 0:
        //OFF STATE
        break;
    case 1:
        //STATE 1 Manual Rear strobe (RED)
        stro.setColor(CRGB::Red);
        stro.strobeLight();
        break;
    case 2:
        //STATE 2  Manual Front strobe (WHITE)
        stro.setColor(CRGB::White);
        stro.strobeLight();
        break;
    case 3:
        //STATE 3 Automatic Rear Mode Rear (RED)
        //LEDs are solid during a braking event. Return to strobe when riding
        stro.setColor(CRGB::Red);
        bool x = true;
        int i = 0;
        while (x)
        {
            if(i==0){
                MA[i].setMMA(MA[0]); //fetch acceleration and set Moving average 
            } else {
                MA[i].setMMA(MA[i-1]);
            }
            //check for braking
            if (MA[i].isbraking())
            {
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
                MA[0]=MA[array_size-1];
                i = 0;
            }
            delay(update_delay);
            M5.update();
        }
        break;
    case 4:
        stro.setColor(CRGB::White);
        bool x = true;
        int i = 0;
        while (x)
        {
            if(i==0){
                MA[i].setMMA(MA[0]); //fetch acceleration and set Moving average 
            } else {
                MA[i].setMMA(MA[i-1]);
            }
            //check for braking
            if (MA[i].isbraking())
            {
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
                MA[0]=MA[array_size-1];
                i = 0;
            }
            delay(update_delay);
            M5.update();
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


/*bool isbraking(float *arr, int si)
{
    for (int i = 0; i < si; i++)
    {
        if (arr[i] == 0)
        {
            return false;
        }
    }
    float percentage_diff = (arr[si - 1] - arr[0]) / arr[0] * 100;
    if (percentage_diff > percentacc)
    {
        return true;
    }
    else
    {
        return false;
    }
} //checks if the motion is braking given acceleration data
*/