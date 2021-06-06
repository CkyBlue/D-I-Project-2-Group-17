#pragma once

#include "text.h"
#include "pause.h"

float round_to_2dp(float num)
{
    int t = num * 100;
    return t / 100.0f;
}

CustomText tx;

String tempString = "";
int textCursor = 0;
int textScrollDelay = 500;         // milliseconds
unsigned int textColor = 0xffffff; // White
bool textBlinkFlag = false;
int textBlinkDelay = 400;

// TODO Accomodate current Unit
void setTempText(float temp_in_cel)
{
    tempString = String(round_to_2dp(temp_in_cel)) + "*C";
    textCursor = 0;
    textBlinkFlag = false;
    textColor = tempToColor(temp_in_cel);
}

void scrollTempText()
{
    textBlinkFlag = !textBlinkFlag;

    if (tempString.length() == 0) { pause(textScrollDelay); return; }

    if (!textBlinkFlag) { pause(textBlinkDelay); return; }

    if (textCursor >= tempString.length())
    { // Blank at wrap-around
        textCursor = -1;
        Serial.print("\n");
    }
    else
    {
        char c = tempString.charAt(textCursor);
        tx.print(c, textColor);
        Serial.print(String(c));
    }

    textCursor++;
    
    pause(textScrollDelay); 
    return; 
}
