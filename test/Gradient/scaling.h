#pragma once

int max_temp = 50;
int min_temp = -5;

float mapScales(float originMin, float originMax, float val, float targetMin, float targetMax)
{
  return (((val - originMin) / (originMax - originMin)) * (targetMax - targetMin)) + targetMin;
}

int tempToGradientIndex(int temp)
{
  return round(mapScales(min_temp, max_temp, temp, 0, 15));
}