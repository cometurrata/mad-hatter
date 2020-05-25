#pragma once

#include <SmingCore.h>

#include <Libraries/SparkFun_APDS9960/SparkFun_APDS9960.h>
#include "ledcontroller.h"

class GestureSensorClass
{
public:
    void init(LedController *ledController);
    bool wasPatternEncountered();

private:
    Timer taskTimer;
    void task();
    bool patternEncountered = false;
    int nextGestureIndex = 0;
    // uint8_t password[4] = {1, 2, 3, 6};
    const uint8_t password[4] = {LED_LEFT, LED_RIGHT, LED_UP, LED_DOWN};

    typedef enum
    {
        DIR_NONE = 0,
        DIR_LEFT,
        DIR_RIGHT,
        DIR_UP,
        DIR_DOWN,
        DIR_NEAR,
        DIR_FAR,
        DIR_ALL,
        DIR_MAX,
    } Gesture_t;
    String describeGesture(Gesture_t gesture);

    const Gesture_t pattern[6] = {DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_DOWN, DIR_MAX};

    SparkFun_APDS9960 apds;
    void showGestureDirectionForDuration(Gesture_t gesture, uint32_t timeout);
    Timer showGestureDirectionTimer;

    int ledIdForGestureDirection(Gesture_t gesture);
    void startPasswordTimer(int timeout);
    void startShowingPassword();
    void showPasswordTask();

    Timer showPasswordTimer;

    int showPasswordStep = 0;

    LedController *ledController = nullptr;
};

extern GestureSensorClass GestureSensor;
