#pragma once

#include <SmingCore.h>


class CharliePlexing
{
public:
    CharliePlexing &init();
    CharliePlexing &setLed(int position);
    CharliePlexing &turnOff();

private:
    static char activationMap[12][4];
    uint8_t pins[4] = {12, 13, 14, 16};
};