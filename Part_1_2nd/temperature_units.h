#pragma once

/* 
    Handles Temperature Unit maths and transition
*/

enum Unit {C = 0, K = 1, F = 2};
char unitChar[3] = {'C', 'K', 'F'};

int currentUnit = Unit::C;

void incUnit() { if (currentUnit < 2) currentUnit++; else currentUnit = 0; }

float cel_to_kel(float temp_in_cel){
    return temp_in_cel + 273.15;
}

float cel_to_fah(float temp_in_cel){
    return (temp_in_cel * 9/5.0f) + 32;
}

String getTempString(float temp_in_cel){
    switch (currentUnit){
        case Unit::C: return String(temp_in_cel) + "*C";
        case Unit::K: return String(cel_to_kel(temp_in_cel)) + "K";
        case Unit::F: return String(cel_to_fah(temp_in_cel)) + "*F";
    }

    throw String("Invalid Unit!");
}
