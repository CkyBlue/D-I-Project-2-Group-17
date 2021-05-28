#include "M5Atom.h"

uint8_t DisBuff[2 + 5 * 5 * 3];

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
    }
}

//void setSpecial(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
//{
//    DisBuff[0] = 0x05;
//    DisBuff[1] = 0x05;
//    for (int i = 0; i < 25; i++)
//    {
//      if ((i+1)%5 == 0 || i < 5){
//        DisBuff[2 + i * 3 + 0] = Rdata;
//        DisBuff[2 + i * 3 + 1] = Gdata;
//        DisBuff[2 + i * 3 + 2] = Bdata;
//        } else {
//        DisBuff[2 + i * 3 + 0] = 0;
//        DisBuff[2 + i * 3 + 1] = 0;
//        DisBuff[2 + i * 3 + 2] = 0; 
//        }
//    }
//}

void setup()
{
    M5.begin(true, false, true);
    delay(10);
    setBuff(0xff, 0x00, 0x00);
    M5.dis.displaybuff(DisBuff);
}

uint8_t FSM = 0;

void loop()
{
    if (M5.Btn.wasPressed())
    {

        switch (FSM)
        {
        case 0:
            setBuff(0x255, 0x00, 0x00);
            Serial.printf("Red");
            break;
        case 1:
            setBuff(0x00, 0x255, 0x00);
            Serial.printf("Green");
            break;
        case 2:
            setBuff(0x00, 0x00, 0x255);
            Serial.printf("Blue");
            break;
        case 3:
            //setSpecial(0x255, 0x255, 0x255);
            Serial.printf("Black & White");
            break;
        case 4:
            //M5.dis.MoveDisPlayBuff(2, 1);
            Serial.printf("Offset");
            break;
        default:
            break;
        }
        M5.dis.displaybuff(DisBuff);

        FSM++;
        if (FSM >= 3)
        {
            FSM = 0;
        }
    }

    delay(50);
    M5.update();
}
