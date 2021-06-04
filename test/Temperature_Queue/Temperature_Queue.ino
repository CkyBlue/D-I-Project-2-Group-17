#include "pause.h"
#include "temperature_record.h""
#include "M5Atom.h"

void setup() {
  M5.begin(true, true, true);
  M5.IMU.Init();
  
  delay(10);
}

void loop() {
  M5.update();
  if (M5.Btn.wasPressed())
    Serial.printf("Avg Temp: %.2f \n", getAverageTemperature());
  
  if (isPaused()) return;
  
  updateTemperatureData();
  enqueueTemperatureData();

  pause(samplingDelay * 1000);
}
