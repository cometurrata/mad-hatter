#pragma once

#include <SmingCore.h>


class CharliePlexing
{    Wire.pins(4, 5); // SDA, SCL
    mcp.begin();
public:
    CharliePlexing &init();
    CharliePlexing &setLed(int position);
    CharliePlexing &turnOff();
    void task();

private:
    static char activationMap[12][4];
    uint8_t pins[4] = {12, 13, 14, 16};
};