#include "charlieplexing.h"

// 'i' stands for "input" or "hgh (i)mpedance"
// 'h' stands for "output (h)igh"
// 'l' stands for "output (l)ow"
char CharliePlexing::activationMap[12][4] = {
    {'h', 'i', 'i', 'l'}, // 1:  0
    {'l', 'i', 'i', 'h'}, // 2:  1
    {'h', 'i', 'l', 'i'}, // 3:  2
    {'l', 'i', 'h', 'i'}, // 4:  3
    {'h', 'l', 'i', 'i'}, // 5:  4
    {'l', 'h', 'i', 'i'}, // 6:  5
    {'i', 'h', 'i', 'l'}, // 7:  6
    {'i', 'l', 'i', 'h'}, // 8:  7
    {'i', 'h', 'l', 'i'}, // 9:  8
    {'i', 'l', 'h', 'i'}, // 10: 9
    {'i', 'i', 'l', 'h'}, // 11:  10
    {'i', 'l', 'i', 'h'}, // 12:  11
};

CharliePlexing &CharliePlexing::instance()
{
    static CharliePlexing instance = CharliePlexing();
    return instance;
}

CharliePlexing &CharliePlexing::init()
{
    turnOff();
    return instance();
}

CharliePlexing &CharliePlexing::setLed(int figureToLightUp)
{
    for (int i = 0; i < 4; i++)
    {
        switch (CharliePlexing::activationMap[figureToLightUp][i])
        {
        case 'i':
            pinMode(pins[i], INPUT);
            break;
        case 'l':
            pinMode(pins[i], OUTPUT);
            digitalWrite(pins[i], LOW);
            break;
        case 'h':
            pinMode(pins[i], OUTPUT);
            digitalWrite(pins[i], HIGH);
            break;
        default:
            break;
        }
    }
    return instance();  
}

CharliePlexing &CharliePlexing::turnOff()
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(pins[i], INPUT);
    }
    return instance();
}
