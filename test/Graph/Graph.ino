#include "M5Atom.h"

unsigned int heights[] = {4, 5, 2, 1, 3, 2, 1, 2};
int arr_size = 8;

void setup()
{
    M5.begin(true, true, true);
    M5.IMU.Init();

    delay(10);
}

float scrollDelay;

int idx = 0;

void loop()
{ 
    M5.dis.clear();
  
    if (idx <= arr_size - 5){
      idx++;  

      if (idx > arr_size - 5){
          delay(2000);
          idx = 0;
        }
    }

    for (int i=0; i < 5; i++){
      int height = heights[idx + i];
      M5.dis.drawpix(5*(5 - height) + i, 0xffffff);  
    }

    delay(500);
    M5.update();
}
