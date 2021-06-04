#include "orientation.h"
#include "modes.h"

#include "M5Atom.h"

int updateDelay = 50;

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

      if (state != -1)
         Serial.print("\nMode - " + modesText[state] + "\n");
   }

   delay(updateDelay);
   M5.update();
}
