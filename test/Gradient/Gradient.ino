#include "M5Atom.h"

int max_temp = 50;
int min_temp = -5;

// Size = 16
unsigned int gradient[] = {
0x5350fd, 0x7454fc, 0x8f59fd, 0xa961fe, 0xc563ff,
0xe26afe, 0xe57fd7, 0xe88bb8, 0xeca191, 0xedb668,
0xeec34d, 0xddd13d, 0xbddb3f, 0x9be344, 0x79ec46, 0x50f54c
};

void setup()
{
  M5.begin(true, true, true);
  M5.IMU.Init();

  M5.dis.clear();
  //displayGradient(true);
  delay(10);
}

unsigned int toGrid(int x, int y) { return 5 * y + x; }

// idx - 0 to 15, Maps to circle
unsigned int mapCircular(unsigned int idx)
{
  if (idx < 5)
    return toGrid(0, 4 - idx);
  else if (idx < 9)
    return toGrid(idx - 4, 0);
  else if (idx < 13)
    return toGrid(4, idx - 8);
  else
    return toGrid(16 - idx, 4);
}

void displayGradient(bool step)
{
  Serial.print("Gradient:\n");
  for (int i = 0; i < 16; i++)
  {
    unsigned int color = gradient[i];
    M5.dis.drawpix(mapCircular(i), color);
    //Serial.printf("%.2f, %.2f \n", mapCircular(i), color);

    if (step)
    {
      M5.update();
      delay(50);
    }
  }
}

float mapScales(float originMin, float originMax, float val, float targetMin, float targetMax)
{
  return (((val - originMin) / (originMax - originMin)) * (targetMax - targetMin)) + targetMin;
}

int tempToGradientIndex(int temp)
{
  return round(mapScales(min_temp, max_temp, temp, 0, 15));
}

int cur_temp = 25;

void showCenter(unsigned int color)
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      int y = i + 1;
      int x = j + 1;
      M5.dis.drawpix(toGrid(x, y), color);
    }
  }
}

void loop()
{
  M5.dis.clear();
  displayGradient(true);
  delay(3000);
  
//  M5.dis.clear();
//  displayGradient(false);
//  showCenter(gradient[tempToGradientIndex(cur_temp)]);
//
//  M5.update();
//  delay(500);
//
//  M5.dis.drawpix(mapCircular(tempToGradientIndex(cur_temp)), 0x000000);
//  M5.update();
//  delay(500);
}
