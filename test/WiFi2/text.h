#pragma once
#include "M5Atom.h"

// '*'s are intepreted as degrees symbol
class CustomText{
  private:
  
  int8_t px_0[17] = {0, 1, 2, 3, 4, 5, 9, 10, 14, 15, 19, 20, 21, 22, 23, 24, -1};
  int8_t px_1[12] = {0, 1, 2, 7, 12, 17, 20, 21, 22, 23, 24, -1};
  int8_t px_2[18] = {0, 1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 15, 20, 21, 22, 23, 24, -1};
  int8_t px_3[18] = {0, 1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, -1};
  int8_t px_4[12] = {0, 4, 5, 9, 10, 11, 12, 13, 14, 19, 24, -1};
  int8_t px_5[18] = {0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, -1};
  int8_t px_6[19] = {0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14, 15, 19, 20, 21, 22, 23, 24, -1};
  int8_t px_7[10] = {0, 1, 2, 3, 4, 9, 14, 19, 24, -1};
  int8_t px_8[20] = {0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 19, 20, 21, 22, 23, 24, -1};
  int8_t px_9[19] = {0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, -1};
  int8_t px_K[11] = {1, 4, 6, 8, 11, 12, 16, 18, 21, 24, -1};
  int8_t px_C[13] = {1, 2, 3, 4, 5, 10, 15, 21, 22, 23, 24, -1};
  int8_t px_F[14] = {0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14, 15, 20, -1};
  int8_t px_deg[9] = {0, 1, 2, 5, 7, 10, 11, 12, -1};
  int8_t px_dot[5] = {17, 18, 22, 23, -1};
  int8_t px_blank[1] = {-1};
  int8_t px_minus[4] = {11, 12, 13, -1};
  int8_t px_g[10] = {2, 6, 8, 12, 13, 15, 18, 21, 22, -1};
  int8_t px_m[12] = {6, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, -1};
  int8_t px_neg_cube[13] = {2, 3, 4, 9, 10, 11, 13, 14, 19, 22, 23, 24, -1};
  int8_t px_percentage[14] = {0, 1, 4, 5, 6, 8, 12, 16, 18, 19, 20, 23, 24, -1};
  
  public:
  void print(char c, unsigned int color){
      M5.dis.clear();
      int8_t* arr = NULL;
      switch(c) {
          case '0': {arr = px_0; break;}
          case '1': {arr = px_1; break;}
          case '2': {arr = px_2; break;}
          case '3': {arr = px_3; break;}
          case '4': {arr = px_4; break;}
          case '5': {arr = px_5; break;}
          case '6': {arr = px_6; break;}
          case '7': {arr = px_7; break;}
          case '8': {arr = px_8; break;}
          case '9': {arr = px_9; break;}
          case 'K': {arr = px_K; break;}
          case 'C': {arr = px_C; break;}
          case 'F': {arr = px_F; break;}
          case '*': {arr = px_deg; break;}
          case '.': {arr = px_dot; break;}
          case ' ': {arr = px_blank; break;}
          case '-': {arr = px_minus; break;}
          case 'g': {arr = px_g; break;}
          case 'm': {arr = px_m; break;}
          case '^': {arr = px_neg_cube; break;}
          case '%': {arr = px_percentage; break;}
          default: break;
    }
    if (arr == NULL) return;
    else {
        int idx = 0;
        while (arr[idx] != -1 && idx < 100){
          M5.dis.drawpix(arr[idx], color);
          idx++;
        }
    }
  }
};
