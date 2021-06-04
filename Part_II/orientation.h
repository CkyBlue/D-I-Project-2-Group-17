#pragma once
#include "M5Atom.h"

enum Dir { UP = 0, LEFT = 1, RIGHT = 2, BACK = 3, FRONT = 4, DOWN = 5 };

int constraint = 20; // Allowance in detecting angles
bool isEqual(int val, int goal) { return val >= goal - constraint && val <= goal + constraint; }

float pitch, roll, yaw;
int getOrientation()
{
   M5.IMU.getAhrsData(&pitch, &roll, &yaw);
   M5.update();

   if (isEqual(pitch, 0) && isEqual(abs(roll), 180)) 
      return Dir::UP;
   else if (isEqual(pitch, 90)) return Dir::LEFT;
   else if (isEqual(pitch, -90)) return Dir::RIGHT;
   else if (isEqual(roll, 90)) return Dir::BACK;
   else if (isEqual(roll, -90)) return Dir::FRONT;
   else if (isEqual(abs(pitch), 0) && isEqual(abs(roll), 0))
      return Dir::DOWN;
   return -1;
}

// Used to ascertain change in Orientation
int currentOrientation = 0;
int prevOrientation = -1;

String orientationText[] = {"Facing Up", "Facing Left", "Facing Right", "LED Facing Towards Holder", "LED Facing Away", "Upside Down"};