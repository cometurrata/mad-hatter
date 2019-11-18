#include "knocker.h"

KnockerClass knocker;
//                                                0  1  2  3  4  5  6  7  8  9
const uint8_t KnockerClass::knockPerFigure[10] = {4, 0, 3, 0, 2, 0, 0, 0, 1, 0};


void KnockerClass::knock()
{
    switch (knockingStep)
    {
        case 0:
            digitalWrite(14, HIGH);
            debugf("Knock !");
            knockingStep = 1;
            timer.initializeMs(75, std::bind(&KnockerClass::knock, this)).startOnce();
            break;

        case 1:
            knockingStep = 0;
            knockToMake--;
            digitalWrite(14, LOW);
            if (knockToMake != 0)
            {
                timer.initializeMs(pauseDuration, std::bind(&KnockerClass::knock, this)).startOnce();
            }
            else
            {
                debugf("Done\n\n");
            }
            break;
        default:
            debugf("Wrong step value : %d", knockingStep);
            break;
    }
}

void KnockerClass::start(uint8_t figure)
{
    timer.stop();

    pinMode(14, OUTPUT);
    digitalWrite(14, LOW);

    knockToMake = knockPerFigure[figure];

    debugf("Figure %d, Knocking %d time(s)", figure , knockToMake);

    if(knockToMake == 0)
    {
        debugf("Not Knocking at all. Aborting current knocking");
        return ;
    }    
    knockingStep = 0;
    timer.initializeMs(50, std::bind(&KnockerClass::knock, this)).startOnce();
}