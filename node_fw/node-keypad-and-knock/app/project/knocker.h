#pragma once

#include <SmingCore.h>
#include <Libraries/MCP23017/MCP23017.h>

class KnockerClass
{
public:
    void start(uint8_t figure);
    void init()
    {
        knockToMake = 0;
        knockingStep = 0;
        pauseDuration = 200;
    }

private:
    uint8_t knockToMake = 0;
    uint8_t knockingStep = 0;

    void knock();
    uint32_t pauseDuration = 200;
    static const uint8_t knockPerFigure[10];
    Timer timer;
};

extern KnockerClass knocker;
