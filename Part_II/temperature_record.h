#pragma once
#include <ArduinoQueue.h>
#include "M5Atom.h"
#include "scaling.h"

// Values outside range are clamped
const int max_temp = 60;
const int min_temp = -15;

float clampTemp(float temp){
  if (temp > max_temp) temp = max_temp;
  else if (temp < min_temp) temp = min_temp;
  return temp;
}

const unsigned int samplesPerHour = 4;

// In Seconds
const unsigned int samplingDelay = 60 * 60 / samplesPerHour;

ArduinoQueue<float> temperatures(samplesPerHour * 24);
float currentTemp = 0;

void updateTemperatureData() { M5.IMU.getTempData(&currentTemp); }
void enqueueTemperatureData() {
  while (temperatures.itemCount() >= temperatures.maxQueueSize()) {temperatures.dequeue(); Serial.print("while loop");}
  temperatures.enqueue(currentTemp);
}

// If data not enough for 24 hours, entries will be -99
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

  for (int i = 0; i < hour; i++) hourlyAverages[i] /= samplesPerHour;
  for (int i = hour; i < 24; i++) hourlyAverages[i] = -99;

  Serial.print("{");
  for (int i = 0; i < 24; i++) Serial.printf(" %.2f,", hourlyAverages[i]);
  Serial.print(" }\n");
}

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

  if (sum == 0) return currentTemp;

  return sum/hr;
}
