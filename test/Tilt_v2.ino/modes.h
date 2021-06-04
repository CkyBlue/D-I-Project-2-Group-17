#pragma once
#include "orientation.h"
#include "M5Atom.h"

String modesText[] = {"I - Show Active Temp", "II - Show 24 Hr Avg", "III - Temperature Gradient", "IV - Graph", "V - Change Units"};
enum Modes { Mode_I = 0, Mode_II = 1, MODE_III = 2, MODE_IV = 3, MODE_V = 4 };

int state = -1;
void incMode() { if (state < 4) state++; else state = 0; }
void decMode() { if (state > 0) state--; else state = 4; }

bool activeDisplay = false;

bool refreshMode(){
   int orientation = getOrientation();
   bool wasOrientationChanged = false;
   bool wasStateChanged = false;

   Serial.print("Orientation: " + String(orientation) + "\n");
   Serial.print("Prev Orientation: " + String(prevOrientation) + "\n");
   
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
            if (prevOrientation == Dir::RIGHT) { incMode(); wasStateChanged = true;}
            if (prevOrientation == Dir::LEFT) { decMode(); wasStateChanged = true;}
         }
         
         if (currentOrientation == Dir::DOWN)
         {
            state = -1;
            activeDisplay = false;

            Serial.print("Display Turned Off\n");
            wasStateChanged = true;
            // M5.dis.clear();
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
         // Serial.print("Mode - " + modesText[state] + "\n");
         wasStateChanged = true;
      }
   }

   return wasStateChanged;
}
