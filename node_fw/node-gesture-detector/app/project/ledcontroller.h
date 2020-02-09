#pragma once

#include "charlieplexing.h"

class LedController
{
    CharliePlexing *charlieplexing = nullptr;
    int currentLedOnId = -1;

public:
    void init(CharliePlexing *cp)
    {
        this->charlieplexing = cp;
        charlieplexing->init();
        pinMode(0, OUTPUT);
        pinMode(15, OUTPUT);

        turnOff(12);
        turnOff(13);
    }

    void turnOff()
    {
        turnOff(currentLedOnId);
    }

    void turnOff(int ledId)
    {
        if (ledId < 0)
        {
            return;
        }

        charlieplexing->turnOff();

        if (ledId == 12)
        {
            digitalWrite(0, HIGH);
        }
        else if (ledId == 13)
        {
            digitalWrite(15, LOW);
        }
        currentLedOnId = -1;
    }

    void turnOn(int ledId)
    {
        if (ledId < 0)
            return;

        if (ledId >= 14)
            return;

        if (ledId < 12)
        {
            charlieplexing->setLed(ledId);
        }
        else if (ledId == 12)
        {
            digitalWrite(0, LOW);
        }
        else if (ledId == 13)
        {
            digitalWrite(15, HIGH);
        }
        currentLedOnId = ledId;
    }
};