#pragma once

#include <SmingCore.h>

#include <Libraries/SparkFun_APDS9960/SparkFun_APDS9960.h>

class GestureSensorClass
{
public:
    void init();
    bool wasPatternEncountered();

private:
    Timer taskTimer;
    void task();
    bool patternEncountered = false;
    int nextGestureIndex = 0;
    typedef enum
    {
        DIR_NONE = 0,
        DIR_LEFT,
        DIR_RIGHT,
        DIR_UP,
        DIR_DOWN,
        DIR_NEAR,
        DIR_FAR,
        DIR_ALL
    } Gesture_t;

    Gesture_t pattern[15] = {DIR_NONE};
};

extern GestureSensorClass GestureSensor;
