#include "orientation.h"
#include "modes.h"
#include "draw.h"
#include "colors.h"
#include "grid.h"
#include "pause.h"
#include "scaling.h"
#include "text.h"
#include "temperature_record.h"

#include "M5Atom.h"

int updateDelay = 50;
CustomText tx;

enum Unit {C = 0, K = 1, F = 2};
Unit currentUnit = Unit::C;

void setup()
{
   M5.begin(true, true, true);
   M5.IMU.Init();

   M5.dis.clear();
   delay(10);
}

float round_to_2dp(float num){
   int t = num * 100;
   return t/100.0f; 
}

// <-- Intra-Mode Tracking
// Mode I & 2
String tempString = "";
int textCursor = 0;
int textScrollDelay = 500; // milliseconds
int textColor = 0xffffff; // White

// TODO Accomodate current Unit
void setTempText(float temp_in_cel){
   tempString = String(round_to_2dp(temp_in_cel)) + "*C";
   textCursor = 0;
   textColor = tempToColor(currentTemp);
}

void scrollTempText(){
   if (tempString.length() == 0) return;

   if (textCursor >= tempString.length()) { // Blank at wrap-around 
      textCursor = -1;
      Serial.print("\n");
   }
   else {
      char c = tempString.charAt(textCursor);
      tx.print(c, textColor);
      Serial.print(String(c));
   }

   textCursor++;
}

// Mode III
int currentTempColorPos = 0;
bool gradientBlinkFlag = true;
int gradientBlinkDelay = 250; // milliseconds

// Mode IV
int graphXCursor = 0;
int graphScrollDelay = 500;
int graphWrapDelay = 2000;

bool scrollGraph(){
   hourlyAverages; 

   // If greater than 24 or at rouge value
   if (graphXCursor > (24 - 5) || (hourlyAverages[graphXCursor] < -98.5 && hourlyAverages[graphXCursor] > -99.5)) {
      graphXCursor = 0;
      return false;
   }

   int temp;
   for (int i = 0; i < 5; i++) {
      temp = hourlyAverages[graphXCursor + i];

      int height = getBarHeightFromTemp(temp);
      int index = round(mapScales(1, 5, height, 0, 15));
      unsigned int color = gradient[index];

      drawBar(i, height, color);
   }
   graphXCursor++;

   return true;
}

// --> 

void loop()
{
   bool wasStateChanged = refreshMode(); // Handles all mode transition related stuff

   if (wasStateChanged){
      resetPause();

      Serial.print("\nMode - " + modesText[state] + "\n");

      switch (state)
      {
      case Mode_I: { setTempText(currentTemp); break; }
      case Mode_II: { setTempText(getAverageTemperature()); break; }
      
      // Temperature Gradient
      case MODE_III: { currentTempColorPos = mapCircular(tempToGradientIndex(currentTemp)); break; }

      case MODE_IV: { graphXCursor = 0; updateHourlyAverages(); break; } // Temperature Graph
      case MODE_V: { M5.dis.clear(); break; } // Off

      default:
         break;
      }
   }

   // Custom Delay Implementation
   if (isPaused()) return;

   if (!activeDisplay)
   { // Background Reading mode
      updateTemperatureData();
      enqueueTemperatureData();

      pause(samplingDelay * 1000);
      M5.update();
      return;
   }
   else
   {
      M5.dis.clear();

      switch (state)
      {
      case Mode_I: // Show active Temp
      {
         scrollTempText(); pause(textScrollDelay); return;
      }
      case Mode_II: // Show 24 Hr Avg
      {
         scrollTempText(); pause(textScrollDelay); return;
      }
      case MODE_III: // Temperature Gradient
      {
         displayGradient();

         gradientBlinkFlag = !gradientBlinkFlag;
         if (gradientBlinkFlag){ M5.dis.drawpix(currentTempColorPos, 0x000000); }

         pause(gradientBlinkDelay); return;
      }
      case MODE_IV: { // Temperature Graph
         if (scrollGraph()) 
            pause(graphScrollDelay);
         else
            pause(graphWrapDelay);
         
         return;
      }
      default:
         break;
      }
   }

   delay(updateDelay);
   M5.update();
}
