#pragma once 

// Size = 16
unsigned int gradient[] = {
    0x5350fd, 0x7454fc, 0x8f59fd, 0xa961fe, 0xc563ff,
    0xe26afe, 0xe57fd7, 0xe88bb8, 0xeca191, 0xedb668,
    0xeec34d, 0xddd13d, 0xbddb3f, 0x9be344, 0x79ec46, 0x50f54c};

float mapScales(float originMin, float originMax, float val, float targetMin, float targetMax)
{
  return (((val - originMin) / (originMax - originMin)) * (targetMax - targetMin)) + targetMin;
}

unsigned int toGrid(int x, int y) { return 5 * y + x; }