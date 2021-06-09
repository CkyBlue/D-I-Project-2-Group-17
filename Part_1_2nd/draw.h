#pragma once

#include "M5Atom.h"
#include "colors.h"
#include "grid.h"

// Pos - 0 to 4, Height - 1 to 5
void drawBar(int x_pos, int height, unsigned int color)
{
  for (int i = 0; i < height; i++)
    M5.dis.drawpix(toGrid(x_pos, 4 - i), color);
}

void displayGradient()
{
  for (int i = 0; i < 16; i++)
  {
    unsigned int color = gradient[i];
    M5.dis.drawpix(mapCircular(i), color);
  }
}
