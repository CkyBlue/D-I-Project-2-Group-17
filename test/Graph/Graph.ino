#include "M5Atom.h"

// Size = 16
unsigned int gradient[] = {
    0x5350fd, 0x7454fc, 0x8f59fd, 0xa961fe, 0xc563ff,
    0xe26afe, 0xe57fd7, 0xe88bb8, 0xeca191, 0xedb668,
    0xeec34d, 0xddd13d, 0xbddb3f, 0x9be344, 0x79ec46, 0x50f54c};

unsigned int heights[] = {4, 5, 2, 1, 3, 2, 1, 2};
int arr_size = 8;

float mapScales(float originMin, float originMax, float val, float targetMin, float targetMax)
{
  return (((val - originMin) / (originMax - originMin)) * (targetMax - targetMin)) + targetMin;
}

void setup()
{
  M5.begin(true, true, true);
  M5.IMU.Init();

  delay(10);
}

unsigned int toGrid(int x, int y) { return 5 * y + x; }

void drawBar(int x_pos, int height, unsigned int color)
{
  for (int i = 0; i < height; i++)
    M5.dis.drawpix(toGrid(x_pos, 4 - i), color);
}

float scrollDelay;

int idx = 0;

void loop()
{
  M5.dis.clear();

  if (M5.Btn.wasPressed())
  {
    if (idx <= arr_size - 5)
    {
      idx++;
      if (idx > arr_size - 5)    
        idx = 0;
    }
  }

  for (int i = 0; i < 5; i++)
  {
    int height = heights[idx + i];
    unsigne int color = gradient[round(mapScales(1, 5, height, 0, 15))];
    drawBar(i, height, color);
  }

  delay(500);
  M5.update();
}
