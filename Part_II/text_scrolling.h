#pragma once

#include "text.h"
#include "pause.h"
#include "sensor_data.h"
#include "temperature_units.h"

CustomText tx;

/* 
  Modified "/Part_1_2nd.h/text_scrolling.h" to accomodate the HDC2080 and humidity values
*/

String outputString = "";
int textCursor = 0;
int textScrollDelay = 500;         // milliseconds
unsigned int textColor = 0xffffff; // White
bool textBlinkFlag = false;
int textBlinkDelay = 400;

void setText(float temp_in_cel, float humidity)
{
    outputString = getTempString(temp_in_cel) + " " + String(humidity) + "%";
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
