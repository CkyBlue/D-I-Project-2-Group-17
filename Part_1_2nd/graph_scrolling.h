#pragma once

/* 
    Handles graph scrolling. Keeps tracks of scroll cursor, wrap-around and wrap-around/scroll delays
*/

#include "temperature_record.h"
#include "draw.h"
#include "pause.h"

int graphXCursor = 0;
int graphScrollDelay = 500;//millisecs
int graphWrapDelay = 2000;//millisecs

void scrollGraph()
{
    // If greater than 24 or at rouge value. 
    // If all values are rouge values, skips to displaying current temp
    if (graphXCursor > (24 - 5) || ((hourlyAverages[graphXCursor] < -98.5 && hourlyAverages[graphXCursor] > -99.5) && (graphXCursor != 0)))
    {
        graphXCursor = 0;
        pause(graphWrapDelay);

        return;
    }

    int temp;
    for (int i = 0; i < 5; i++)
    {
        temp = hourlyAverages[graphXCursor + i];
        if (temp < -98.5 && temp > -99.5)
        {
            if (graphXCursor == 0 && i == 0)
                temp = currentTemp; // If all values are rogue values, displays current temp
            else
                break;
        };

        int height = getBarHeightFromTemp(temp);
        int index = round(mapScales(1, 5, height, 0, 15));
        unsigned int color = gradient[index];

        drawBar(i, height, color);
    }
    graphXCursor++;

    pause(graphScrollDelay);
    return;
}
