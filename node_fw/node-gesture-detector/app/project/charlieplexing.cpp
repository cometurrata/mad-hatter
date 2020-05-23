#include "charlieplexing.h"

// 'i' stands for "input" or "hgh (i)mpedance"
// 'h' stands for "output (h)igh"
// 'l' stands for "output (l)ow"
char CharliePlexing::activationMap[14][5] = {
    {'h', 'i', 'i', 'l', 'i'}, // 1:  0
    {'l', 'i', 'i', 'h', 'i'}, // 2:  1
    {'h', 'i', 'l', 'i', 'i'}, // 3:  2
    {'l', 'i', 'h', 'i', 'i'}, // 4:  3
    {'h', 'l', 'i', 'i', 'i'}, // 5:  4
    {'l', 'h', 'i', 'i', 'i'}, // 6:  5
    {'i', 'h', 'i', 'l', 'i'}, // 7:  6
    {'i', 'l', 'i', 'h', 'i'}, // 8:  7
    {'i', 'h', 'l', 'i', 'i'}, // 9:  8
    {'i', 'l', 'h', 'i', 'i'}, // 10: 9
    {'i', 'i', 'l', 'h', 'i'}, // 11: UP
    {'i', 'l', 'i', 'h', 'i'}, // 12: DOWN
    {'i', 'i', 'i', 'h', 'l'}, // 13: LEFT
    {'i', 'i', 'i', 'l', 'h'}, // 14: RIGHT

};

CharliePlexing &CharliePlexing::init()
{    
    Wire.pins(4, 5); // SDA, SCL
    mcp.begin();
    return *this;
}


CharliePlexing &CharliePlexing::setLed(int figureToLightUp)
{
    for (int i = 0; i < 5; i++)
    {
        switch (CharliePlexing::activationMap[figureToLightUp][i])
        {
        case 'i':
            mcp.pinMode(pins[i], INPUT);
            break;
        case 'l':
            mcp.pinMode(pins[i], OUTPUT);
            mcp.digitalWrite(pins[i], LOW);
            break;
        case 'h':
            mcp.pinMode(pins[i], OUTPUT);
            mcp.digitalWrite(pins[i], HIGH);
            break;
        default:
            break;
        }
    }
    return *this;  
}

CharliePlexing &CharliePlexing::turnOff()
{
    for (int i = 0; i < 5; i++)
    {
        mcp.pinMode(pins[i], INPUT);
    }
    return *this;
}
