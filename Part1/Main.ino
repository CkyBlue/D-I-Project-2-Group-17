#include <Arduino.h>
#include <M5Atom.h>
#include <FastLED.h>
#include <math.h>
void setup()
{
    M5.begin(true, false, true);
}
void strobe(CRGB color) {
    int strobeDelay = 500;
    int updateDelay = 50;

    int strobeSteps = strobeDelay / updateDelay;
    uint8_t strobeCount = 0;
    bool strobeFlag = false;

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
        M5.dis.fillpix(color);
    }
}
float getAvg(float * arr,int si){
    float sum=0;
    for(int i=si;i<si;i++){
        sum+=arr[i];
    }
    return sum/(float)si;
}
void CalibrateSensor(); //callibrate sensor to
int FSM = 1;
float accX = 0, accY = 0, accZ = 0; //acceleration values of the device
void loop()
{
    switch (FSM)
    {
    case 0:
        //OFF STATE
        break;
    case 1:
        //STATE 1 Manual Rear strobe (RED)
        strobe(CRGB::Red);
        break;
    case 2:
        //STATE 2  Manual Front strobe (WHITE)
        strobe(CRGB::White);
        break;
    case 3:
        //STATE 3 Automatic Rear Mode Rear (RED)
        //LEDs are solid during a braking event. Return to strobe when riding
        strobe(CRGB::White);
        bool x = true;int i=0;
        do
        {
            time_t initialt = time(NULL);
            float acca[5]={0,0,0,0,0};
            time_t times[5]={0,0,0,0,0};
                //check for braking
                //IF acceleration is increasing by more than x% relative to 2 timestamps ago
            M5.IMU.getAccelData(&accX, &accY, &accZ);
        
            float acc = sqrt(pow(accX, 2) + pow(accY, 2) + pow(accZ, 2));
            M5.update();
            acca[i]=acc;
            times[i]=initialt;
            float U= getAvg(acca,5); 
            i++;
            if(i>=5){i=0;}
        } while (x);
        break;
    case 4:
        //Automatic Front Mode Rear (WHITE)
        //strobe();
        break;
    default:
        break;
    }
    if (M5.Btn.wasPressed())
    {
        FSM++;
        if (FSM >= 5)
            FSM = 0;
    }
}

/*strobe(CRGB color)
{

    int strobeDelay = 500;
    int updateDelay = 50;

    int strobeSteps = strobeDelay / updateDelay;
    uint8_t strobeCount = 0;
    bool strobeFlag = false;

    strobeCount++;
    if (strobeCount >= strobeSteps)
    {
        strobeCount = 0;
        strobeFlag = !strobeFlag;
    }
    if (strobeFlag)
    {
        M5.dis.fillpix(0xf00000);
    }
    else
    {
        M5.dis.drawpix(1, 0x000000);
    }
}*/