#include "knocker.h"

KnockerClass::KnockerClass()
{
    pinMode(0, OUTPUT);
    digitalWrite(0, LOW);
}

void KnockerClass::knock()
{
    switch (knockingStep)
    {
    case 0:
        digitalWrite(0, HIGH);
        timer.initializeMs(50, std::bind(&KnockerClass::knock, this)).startOnce();
        break;

    case 1:
        knockToMake--;
        digitalWrite(0, LOW);
        if (knockToMake != 0)
        {
            timer.initializeMs(pauseDuration, std::bind(&KnockerClass::knock, this)).startOnce();
        }
        break;
    }
    knockingStep = (knockingStep + 1) % 2;
}

void KnockerClass::start(int figure)
{
    timer.stop();
    knockToMake = knockPerFigure[figure];
    knockingStep = 0;
    timer.initializeMs(50, std::bind(&KnockerClass::knock, this)).startOnce();
    digitalWrite(0, LOW);
}