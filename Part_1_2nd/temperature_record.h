#pragma once

/* 
  Reads, stores and analyzes temperature data 
*/

#include <ArduinoQueue.h>
#include "M5Atom.h"
#include "scaling.h"

// Values outside range are clamped
const int max_temp = 60;
const int min_temp = -20;

float clampTemp(float temp){
  if (temp > max_temp) temp = max_temp;
  else if (temp < min_temp) temp = min_temp;
  return temp;
}

// Sampling parameters
const unsigned int samplesPerHour = 4;
const unsigned int samplingDelay = 60 * 60 / samplesPerHour; // In Seconds

// Queue of 24 hours of data
ArduinoQueue<float> temperatures(samplesPerHour * 24);

// Global current temperature parameter
float currentTemp = 0;

void updateTemperatureData() { M5.IMU.getTempData(&currentTemp); }
void enqueueTemperatureData() {
  while (temperatures.itemCount() >= temperatures.maxQueueSize()) {temperatures.dequeue(); Serial.print("while loop");}
  temperatures.enqueue(currentTemp);
}

// Analyzes 24 hours worth of data to determine average temperature of each hour
// If data not enough for 24 hours, entries will be at rouge value of -99
float hourlyAverages[24]= {0};
void updateHourlyAverages(){
  ArduinoQueue<float> temp(temperatures.itemCount());

  float temperature;

  for (int i = 0; i < 24; i++) hourlyAverages[i] = 0;

  unsigned int hour = 0, counter = 0;
  Serial.print("[");
  while (!temperatures.isEmpty()){
    temperature = temperatures.dequeue();
    Serial.printf(" %.2f,", temperature);

    // Array will store sums only at first
    hourlyAverages[hour] += temperature; 

    counter++;
    if (counter >= samplesPerHour) { 
      counter = 0; 
      hour++; 

      if (hour >= 24) break;
    }
    
    temp.enqueue(temperature);
  } 
  Serial.print(" ]\n");

  while (!temp.isEmpty()){
    temperatures.enqueue(temp.dequeue());  
  }

  // Calculating averages
  for (int i = 0; i < hour; i++) hourlyAverages[i] /= samplesPerHour;
  for (int i = hour; i < 24; i++) hourlyAverages[i] = -99; // Rouge values

  Serial.print("{");
  for (int i = 0; i < 24; i++) Serial.printf(" %.2f,", hourlyAverages[i]);
  Serial.print(" }\n");
}

// Average of the averages
float getAverageTemperature(){
  updateHourlyAverages();

  float hourlyAvg, sum = 0;
  int hr = 0;
  for (; hr < 24; hr++){
    hourlyAvg = hourlyAverages[hr];
    if (hourlyAvg < -98.5 && hourlyAvg > -99.5) // Is ~99.0
      break;

    sum += hourlyAvg;
  }

  // If less than an hour has passed, the current temperature will be used as the average temperature
  if (sum == 0) return currentTemp; 

  return sum/hr;
}
