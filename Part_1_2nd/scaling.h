#pragma once

#include "temperature_record.h"
#include "colors.h"

/* 
  Manages scaling temperature to gradient-color/bar-height values
*/

// Maps a value within a given range to a corresponding value within another range
float mapScales(float originMin, float originMax, float val, float targetMin, float targetMax)
{
  return (((val - originMin) / (originMax - originMin)) * (targetMax - targetMin)) + targetMin;
}

int tempToGradientIndex(float temp)
{
  return round(mapScales(min_temp, max_temp, clampTemp(temp), 0, 15));
}

int tempToColor(float temp){
  return gradient[tempToGradientIndex(temp)];
}

int getBarHeightFromTemp(float temp){
  return round(mapScales(min_temp, max_temp, clampTemp(temp), 1, 5));
}
