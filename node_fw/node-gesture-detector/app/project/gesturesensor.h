#pragma once

#include <SmingCore.h>

#include <Libraries/SparkFun_APDS9960/SparkFun_APDS9960.h>
#include "charlieplexing.h"

class GestureSensorClass
{
public:
    void init(CharliePlexing *charlieplexing);
    bool wasPatternEncountered();

private:
    Timer taskTimer;
    void task();
    bool patternEncountered = false;
    int nextGestureIndex = 0;
    uint8_t password[4] = {1, 2, 3, 6};
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

    Gesture_t pattern[15] = {DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_DOWN};

    void startShowingPassword();
    void showPasswordTask();

    Timer showPasswordTimer;

    int showPasswordStep = 0;

    CharliePlexing *charlieplexing = nullptr;
};

extern GestureSensorClass GestureSensor;
