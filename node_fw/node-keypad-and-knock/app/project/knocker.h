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

    void runPattern(uint16_t *pattern, int patternLen);

private:
    Vector<uint16_t> pattern;

    uint8_t knockToMake = 0;
    uint8_t knockingStep = 0;

    void knock();
    uint32_t pauseDuration = 200;
    static const uint8_t knockPerFigure[10];
    Timer timer;
};

class PatternKnocker
{
public:
    void setPattern(uint16_t *pattern, int patternLen);
    void run();

private:
    void knockNow();
    void releaseKnocker();
    void task();

    int patternIndex = 0;

    Vector<uint16_t> pattern;
    Timer releaseTimer;
    Timer patternTimer;
};

extern KnockerClass Knocker;
