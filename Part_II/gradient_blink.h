#pragma once

#include "M5Atom.h"
#include "pause.h"
#include "draw.h"

int currentTempColorPos = 0;
bool gradientBlinkFlag = true;
int gradientBlinkDelay = 250; // milliseconds

void blinkGradient(){
    displayGradient();

    gradientBlinkFlag = !gradientBlinkFlag;
    if (gradientBlinkFlag){ M5.dis.drawpix(currentTempColorPos, 0x000000); }

    pause(gradientBlinkDelay);
}
