#include "modes.h"
#include "pause.h"
#include "scaling.h"
#include "text.h"
#include "graph_scrolling.h"
#include "text_scrolling.h"
#include "gradient_blink.h"
#include "temperature_record.h"
#include "temperature_units.h"

#include "M5Atom.h"

bool standByFlag = true;
unsigned int long lastSampled = 0;

void setup()
{
   M5.begin(true, true, true);
   M5.IMU.Init();

   updateTemperatureData();
   enqueueTemperatureData();

   M5.dis.clear();
   delay(10);
}

void loop()
{
   bool wasStateChanged = refreshMode(); // Handles all mode transition related stuff

   if (wasStateChanged){
      resetPause();

      if (state != -1){
         Serial.print("\nMode - " + modesText[state] + "\n");
         standByFlag = true;
      }

      switch (state)
      {
      case Mode_I: { setTempText(currentTemp); break; }
      case Mode_II: { setTempText(getAverageTemperature()); break; }
      case MODE_III: { currentTempColorPos = mapCircular(tempToGradientIndex(currentTemp)); break; }
      case MODE_IV: { graphXCursor = 0; updateHourlyAverages(); break; } // Temperature Graph
      case MODE_V: { break; }
      case -1: { M5.dis.clear(); break;} // Off

      default:
         break;
      }
   }

   if (isPaused()) return; // Pause Interception

   // Background reading
   if ((millis() - lastSampled) > (samplingDelay * 1000)) {
      updateTemperatureData();
      enqueueTemperatureData();

      lastSampled = millis();
   }
   
   if (activeDisplay)
   {
      M5.dis.clear();

      if (standByFlag){
         if (M5.Btn.wasPressed()) {
            standByFlag = false;
            return;
         }

         M5.dis.drawpix(toGrid(state, 2), 0xffffff);
         return;
      }

      switch (state)
      {
      case Mode_I: { scrollTempText(); return; }
      case Mode_II: { scrollTempText(); return; }
      case MODE_III: { blinkGradient(); return; }
      case MODE_IV: { scrollGraph(); return; }
      case MODE_V: { showUnit(); if (M5.Btn.wasPressed()) incUnit(); return; }
      default: break;
      }
   }
}
