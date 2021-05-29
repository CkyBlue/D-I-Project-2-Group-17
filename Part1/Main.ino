#include <M5Atom.h>
#include <FastLED.h>
#include <math.h>
#include <thread>
#define percentchange 10 //the percentage change over 2 seconds for it to be braking
#define update_delay 50
#define strobe_delay 500
using namespace std;

//Initialize Variables
int FSM = 1;float U,acc,percentacc;float acca[5]={0,0,0,0,0};
float accX = 0, accY = 0, accZ = 0; //acceleration values of the device
int strobeDelay,updateDelay,strobeSteps;
CRGB color;
uint8_t strobeCount;bool strobeFlag;
void resetstrobe(){
    strobeCount = 0;
    strobeFlag = false;
    strobeDelay=strobe_delay;
    updateDelay=update_delay;
}
void strobe(CRGB &color,uint8_t &strobeCount,bool &strobeFlag) {

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
float getAvg(float * arr,int si){
    float sum=0;
    for(int i=si;i<si;i++){
        sum+=arr[i];
    }
    return sum/(float)si;
}
bool isbraking(float * arr,int si){
    for(int i=0;i<si;i++){
        if(arr[i]==0){
            return false;
        }
    }
    float percentage_diff=(arr[si-1]-arr[0])/arr[0]*100;
    if(percentage_diff>percentacc){
        return true;
    } else {
        return false;
    }
}//checks if the motion is braking given acceleration data

void setup()
{
    M5.begin(true, false, true);
    resetstrobe();
    percentacc=update_delay/1000*percentchange/2;
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
        color=CRGB::Red;
        strobe(color,strobeCount,strobeFlag);
        break;
    case 2:
        //STATE 2  Manual Front strobe (WHITE)
        color=CRGB::White;
        //strobe(color);
        break;
    case 3:
        //STATE 3 Automatic Rear Mode Rear (RED)
        //LEDs are solid during a braking event. Return to strobe when riding
        color=CRGB::Red;
        bool x = true;int i=0;
        while(x)
        {
            //time_t initialt = time(NULL);
            
            //time_t times[5]={0,0,0,0,0};
                //check for braking
                //IF acceleration is increasing by more than x% relative to 2 timestamps ago
            M5.IMU.getAccelData(&accX, &accY, &accZ);
            acc = sqrt(pow(accX, 2) + pow(accY, 2) + pow(accZ, 2));
            acca[i]=acc;
            if(isbraking){
                M5.dis.fillpix(color);
            }
            else {
                strobe(color,strobeCount,strobeFlag);
            }
            i++;
            if(M5.Btn.wasPressed()){break;}
            if(i>=5){i=0;}
            delay(updateDelay);
            M5.update();
        } 
        break;
    case 4:
        //Automatic Front Mode Rear (WHITE)
        color=CRGB::White;
        bool x = true;int i=0;
        while(x)
        {
            //time_t initialt = time(NULL);
            
            //time_t times[5]={0,0,0,0,0};
                //check for braking
                //IF acceleration is increasing by more than x% relative to 2 timestamps ago
            M5.IMU.getAccelData(&accX, &accY, &accZ);
            acc = sqrt(pow(accX, 2) + pow(accY, 2) + pow(accZ, 2));
            acca[i]=acc;
            if(isbraking){
                M5.dis.fillpix(color);
            }
            else {
                strobe(color,strobeCount,strobeFlag);
            }
            i++;
            if(M5.Btn.wasPressed()){break;}
            if(i>=5){i=0;}
            delay(updateDelay);
            M5.update();
        } 
        break;
    default:
        break;
    }
    if (M5.Btn.wasPressed())
    {
        FSM++;
        resetstrobe();
        if (FSM >= 5)
            FSM = 0;
    }
    delay(updateDelay);
    M5.update();
}

