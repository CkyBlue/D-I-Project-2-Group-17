#include "modes.h"
#include "pause.h"
#include "scaling.h"
#include "text.h"
#include "graph_scrolling.h"
#include "text_scrolling.h"
#include "gradient_blink.h"
#include "temperature_record.h"

#include "M5Atom.h"

enum Unit {C = 0, K = 1, F = 2};
Unit currentUnit = Unit::C;

void setup()
{
   M5.begin(true, true, true);
   M5.IMU.Init();

   M5.dis.clear();
   delay(10);
}

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
      case MODE_III: { currentTempColorPos = mapCircular(tempToGradientIndex(currentTemp)); break; }
      case MODE_IV: { graphXCursor = 0; updateHourlyAverages(); break; } // Temperature Graph
      case MODE_V: { break; }
      case -1: { M5.dis.clear(); break;} // Off

      default:
         break;
      }
   }

   if (isPaused()) return; // Pause Interception

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
      case Mode_I: { scrollTempText(); return; }
      case Mode_II: { scrollTempText(); return; }
      case MODE_III: { blinkGradient(); return; }
      case MODE_IV: { scrollGraph(); return; }
      default: break;
      }
   }
}
