#pragma once

#include <SmingCore.h>
#include <Libraries/MCP23017/MCP23017.h>

class KnockerClass
{
public:
    KnockerClass();
    void start(int figure);

private:
    int knockToMake = 0;
    int knockingStep = 0;

    void knock();
    uint32_t pauseDuration = 400;
    //                        0  1  2  3  4  5  6  7  8  9
    int knockPerFigure[10] = {4, 0, 3, 0, 2, 0, 0, 0, 1, 0};
    Timer timer;
};

extern KnockerClass knocker;
