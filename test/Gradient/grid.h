#pragma once

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