#pragma once

#include "text.h"
#include "pause.h"
#include "temperature_units.h"

float round_to_2dp(float num)
{
    int t = num * 100;
    return t / 100.0f;
}

CustomText tx;

String outputString = "";
int textCursor = 0;
int textScrollDelay = 500;         // milliseconds
unsigned int textColor = 0xffffff; // White
bool textBlinkFlag = false;
int textBlinkDelay = 400;

void setText(float temp_in_cel, float humidity)
{
    // TODO Add Humidity
    outputString = getoutputString(temp_in_cel) + " " + String(humidity) + "gm^";
    textCursor = 0;
    textBlinkFlag = false;
}

bool scrollText()
{
    textBlinkFlag = !textBlinkFlag;

    if (outputString.length() == 0) { pause(textScrollDelay); return true; }

    if (!textBlinkFlag) { pause(textBlinkDelay); return true; }

    if (textCursor >= outputString.length())
    { 
        return false;
        Serial.print("\n");
    }
    else
    {
        char c = outputString.charAt(textCursor);
        tx.print(c, textColor);
        Serial.print(String(c));
    }

    textCursor++;
    
    pause(textScrollDelay); 
    return true; 
}

void showUnit(){
    tx.print(unitChar[currentUnit], 0xffffff);
}
