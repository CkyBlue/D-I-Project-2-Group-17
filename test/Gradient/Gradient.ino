#include "colors.h"
#include "grid.h"
#include "scaling.h"
#include "M5Atom.h"

void setup()
{
  M5.begin(true, true, true);
  M5.IMU.Init();

  M5.dis.clear();
  //displayGradient(true);
  delay(10);
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
