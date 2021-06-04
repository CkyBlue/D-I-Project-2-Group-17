#include "M5Atom.h"
#include "text.h"

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
        tx.print(c, 0xffffff);
        delay(500);
    }

    M5.dis.clear();
    delay(500);
    M5.update();
}
