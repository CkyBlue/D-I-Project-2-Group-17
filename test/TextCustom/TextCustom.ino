#include "M5Atom.h"

class CustomText{
  private:
  
  int px_0[17] = {0, 1, 2, 3, 4, 5, 9, 10, 14, 15, 19, 20, 21, 22, 23, 24, -1};
  int px_1[12] = {0, 1, 2, 7, 12, 17, 20, 21, 22, 23, 24, -1};
  int px_2[18] = {0, 1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 15, 20, 21, 22, 23, 24, -1};
  int px_3[18] = {0, 1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, -1};
  int px_4[12] = {0, 4, 5, 9, 10, 11, 12, 13, 14, 19, 24, -1};
  int px_5[18] = {0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, -1};
  int px_6[19] = {0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14, 15, 19, 20, 21, 22, 23, 24, -1};
  int px_7[10] = {0, 1, 2, 3, 4, 9, 14, 19, 24, -1};
  int px_8[20] = {0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 19, 20, 21, 22, 23, 24, -1};
  int px_9[19] = {0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, -1};
  int px_K[11] = {1, 4, 6, 8, 11, 12, 16, 18, 21, 24, -1};
  int px_C[13] = {1, 2, 3, 4, 5, 10, 15, 21, 22, 23, 24, -1};
  int px_F[14] = {0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14, 15, 20, -1};
  int px_deg[9] = {0, 1, 2, 5, 7, 10, 11, 12, -1};
  int px_dot[2] = {17, 18, 22, 23, -1};
  int px_blank[1] = {-1};
  
  public:
  void print(char c){
      M5.dis.clear();
      int* arr = NULL;
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
          default: break;
    }
    if (arr == NULL) return;
    else {
        int idx = 0;
        while (arr[idx] != -1 && idx < 100){
          M5.dis.drawpix(arr[idx], 0xffffff);
          idx++;
        }
    }
  }
};

CustomText tx;
void setup()
{
    M5.begin(true, true, true);
    delay(10);

    M5.IMU.Init();
    M5.dis.clear();
}

String str = "12.13 * C";

void loop()
{
    for (char c: str){
        tx.print(c);
        delay(500);
    }

    M5.dis.clear();
    delay(500);
    M5.update();
}
