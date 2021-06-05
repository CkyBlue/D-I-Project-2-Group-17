#pragma once
#include "temperature_record.h"
#include "colors.h"

float mapScales(float originMin, float originMax, float val, float targetMin, float targetMax)
{
  return (((val - originMin) / (originMax - originMin)) * (targetMax - targetMin)) + targetMin;
}

int tempToGradientIndex(float temp)
{
  Serial.print("Temp: " + String(temp) + "\n");
  Serial.print("Map Scales: " + String(mapScales(min_temp, max_temp, clampTemp(temp), 0, 15)) + "\n");
  return round(mapScales(min_temp, max_temp, clampTemp(temp), 0, 15));
}

int tempToColor(float temp){
  Serial.print("Gradient Index: " + String(tempToGradientIndex(temp)) + "\n");
  return gradient[tempToGradientIndex(temp)];
}

int getBarHeightFromTemp(float temp){
  return round(mapScales(min_temp, max_temp, clampTemp(temp), 1, 5));
}
