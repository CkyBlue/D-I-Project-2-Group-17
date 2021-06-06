#include "M5Atom.h"
#include "pause.h"

enum Dir { UP = 0, LEFT = 1, RIGHT = 2, BACK = 3, FRONT = 4, DOWN = 5 };

float pitch, roll, yaw;
int getOrientation()
{
   M5.IMU.getAhrsData(&pitch, &roll, &yaw);

   if (isEqual(pitch, 0) && isEqual(abs(roll), 180)) 
      return Dir::UP;
   else if (isEqual(pitch, 90)) return Dir::LEFT;
   else if (isEqual(pitch, -90)) return Dir::RIGHT;
   else if (isEqual(roll, 90)) return Dir::BACK;
   else if (isEqual(roll, -90)) return Dir::FRONT;
   else if (isEqual(abs(pitch), 0) && isEqual(abs(roll), 0))
      return Dir::DOWN;
   return -1;
}

// Used to ascertain change in Orientation
int currentOrientation = 0;
int prevOrientation = -1;

int updateDelay = 50;

int constraint = 20; // Allowance in detecting angles
bool isEqual(int val, int goal) { return val >= goal - constraint && val <= goal + constraint; }

String orientationText[] = {"Facing Up", "Facing Left", "Facing Right", "LED Facing Towards Holder", "LED Facing Away", "Upside Down"};
String modesText[] = {"I - Show Active Temp", "II - Show 24 Hr Avg", "III - Temperature Gradient", "IV - Graph", "V - Change Units"};

int state = -1;
void incMode() { if (state < 4) state++; else state = 0; }
void decMode() { if (state > 0) state--; else state = 4; }

bool activeDisplay = false;

void setup()
{
   M5.begin(true, true, true);
   M5.IMU.Init();

   M5.dis.clear();
   delay(10);
}

void loop()
{
   // <-- Orientation Handling Code
   int orientation = getOrientation();
   bool wasOrientationChanged = false;

   if (orientation != -1)
      currentOrientation = orientation;

   wasOrientationChanged = (prevOrientation != currentOrientation);
   if (wasOrientationChanged)
   {
      Serial.print("Orientation - " + orientationText[currentOrientation] + "\n");

      if (activeDisplay)
      {
         if (currentOrientation == Dir::UP)
         {
            if (prevOrientation == Dir::RIGHT) { incMode(); Serial.print("Mode - " + modesText[state] + "\n");}
            if (prevOrientation == Dir::LEFT) { decMode(); Serial.print("Mode - " + modesText[state] + "\n");}
         }
         
         if (currentOrientation == Dir::DOWN)
         {
            state = -1;
            activeDisplay = false;

            Serial.print("Display Turned Off\n");
            M5.dis.clear();
         }
      }
   }

   prevOrientation = currentOrientation;

   if (!activeDisplay && currentOrientation == UP)
   {
      if (M5.Btn.wasPressed())
      {
         activeDisplay = true;
         state = 0;

         Serial.print("Display Turned On\n");
         Serial.print("Mode - " + modesText[state] + "\n");
      }
   }

   if (isPaused()) return;

   // Orientation Handling Code -->

   if (!activeDisplay)
   { // Background Reading mode
   }
   else
   {
      M5.dis.clear();

      switch (state)
      {
      case 0: // Show active Temp
      {
        

         break;
      }
      case 1: // Show 24 Hr Avg
      {

         break;
      }
      case 2: // Temperature Gradient
      {

         break;
      }
      default:
         break;
      }
   }

   delay(updateDelay);
   M5.update();
}
