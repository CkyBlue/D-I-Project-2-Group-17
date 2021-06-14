#pragma once

/* 
  Modified "temperature_record.h" to accomodate the HDC2080 and humidity values
*/

#include <HDC2080.h>

#define ADDR 0x40
HDC2080 sensor(ADDR);

#include <ArduinoQueue.h>
#include "M5Atom.h"

// Values outside range are clamped
const int max_temp = 60;
const int min_temp = -20;

float clampTemp(float temp)
{
  if (temp > max_temp)
    temp = max_temp;
  else if (temp < min_temp)
    temp = min_temp;
  return temp;
}

float round_to_2dp(float num)
{
  int t = num * 100;
  return t / 100.0f;
}

const unsigned int samplesPerHour = 900;

// In Seconds
const unsigned int samplingDelay = 60 * 60 / samplesPerHour;

ArduinoQueue<float> temperatures(samplesPerHour * 24);
float currentTemp = 0, currentHumidity = 0;

void initHDCSensor()
{
  // HDC2080 Initialization

  sensor.begin();
  sensor.reset();

  sensor.setHighTemp(max_temp);
  sensor.setLowTemp(min_temp);

  sensor.setHighHumidity(70);
  sensor.setLowHumidity(30);

  sensor.setMeasurementMode(TEMP_AND_HUMID);
  sensor.setRate(ONE_HZ);
  sensor.setTempRes(FOURTEEN_BIT);
  sensor.setHumidRes(FOURTEEN_BIT);

  sensor.triggerMeasurement();
}

void updateTemperatureData()
{
  // M5.IMU.getTempData(&currentTemp);
  currentTemp = sensor.readTemp();
}

void updateHumidity()
{
  currentHumidity = sensor.readHumidity();
}

void enqueueTemperatureData()
{
  while (temperatures.itemCount() >= temperatures.maxQueueSize())
  {
    temperatures.dequeue();
  }
  temperatures.enqueue(currentTemp);
}

// If data not enough for 24 hours, entries will be -99
float hourlyAverages[24] = {0};
void updateHourlyAverages()
{
  ArduinoQueue<float> temp(temperatures.itemCount());

  float temperature;

  for (int i = 0; i < 24; i++)
    hourlyAverages[i] = 0;

  unsigned int hour = 0, counter = 0;
  Serial.print("[");
  while (!temperatures.isEmpty())
  {
    temperature = temperatures.dequeue();
    Serial.printf(" %.2f,", temperature);

    // Array will store sums only at first
    hourlyAverages[hour] += temperature;

    counter++;
    if (counter >= samplesPerHour)
    {
      counter = 0;
      hour++;

      if (hour >= 24)
        break;
    }

    temp.enqueue(temperature);
  }
  Serial.print(" ]\n");

  while (!temp.isEmpty())
  {
    temperatures.enqueue(temp.dequeue());
  }

  for (int i = 0; i < hour; i++)
    hourlyAverages[i] /= samplesPerHour;
  for (int i = hour; i < 24; i++)
    hourlyAverages[i] = -99;

  Serial.print("{");
  for (int i = 0; i < 24; i++)
    Serial.printf(" %.2f,", hourlyAverages[i]);
  Serial.print(" }\n");
}

String getHourlyAveragesStr()
{
  if (hourlyAverages[0] < -98.5 && hourlyAverages[0] > -99.5)
    return "[" + String(currentTemp) + "]";

  String s = "[" + String(round_to_2dp(hourlyAverages[0]));
  for (int i = 1; i < 24; i++)
  {
    float hourlyAvg = hourlyAverages[i];
    if (hourlyAvg < -98.5 && hourlyAvg > -99.5) // Is ~99.0
      break;

    s += ", " + String(round_to_2dp(hourlyAvg));
  }
  s += "]";

  Serial.print(s + "\n");
  return s;
}

float getAverageTemperature()
{
  updateHourlyAverages();

  float hourlyAvg, sum = 0;
  int hr = 0;
  for (; hr < 24; hr++)
  {
    hourlyAvg = hourlyAverages[hr];
    if (hourlyAvg < -98.5 && hourlyAvg > -99.5) // Is ~99.0
      break;

    sum += hourlyAvg;
  }

  if (sum == 0)
    return currentTemp;

  return sum / hr;
}
