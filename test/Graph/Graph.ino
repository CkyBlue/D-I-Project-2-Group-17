#include "M5Atom.h"
#include "../utils.h"

unsigned int heights[] = {4, 5, 2, 1, 3, 2, 1, 2};
int arr_size = 8;

void setup()
{
  M5.begin(true, true, true);
  M5.IMU.Init();

  delay(10);
}

// Pos - 0 to 4, Height - 1 to 5
void drawBar(int x_pos, int height, unsigned int color)
{
  for (int i = 0; i < height; i++)
    M5.dis.drawpix(toGrid(x_pos, 4 - i), color);
}

float scrollDelay = 500;

int pauseTimer = 0;

bool isPaused()
{
  if (pauseTimer > 0)
  {
    delay(10);
    M5.update();

    pauseTimer -= 10;
    return true;
  }
  return false;
}
void resetPause() { pauseTimer = 0; }
void pause(unsigned int delay) { pauseTimer += delay; }

int idx = 0;
void loop()
{
  
  M5.dis.clear();

  if (idx <= arr_size - 5)
  {
    idx++;
    if (idx > arr_size - 5) {
      idx = 0;
      pause(500);
    }
  }

  for (int i = 0; i < 5; i++)
  {
    int height = heights[idx + i];
    unsigned int color = gradient[round(mapScales(1, 5, height, 0, 15))];
    drawBar(i, height, color);
  }

  pause(500);
  M5.update();
}
