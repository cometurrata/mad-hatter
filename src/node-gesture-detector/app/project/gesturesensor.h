#pragma once

#include <SmingCore.h>
#include <Libraries/Adafruit_NeoPixel/Adafruit_NeoPixel.h>
#include <Libraries/SparkFun_APDS9960/SparkFun_APDS9960.h>
#include "ledcontroller.h"
#include "ui_controller.h"

class GestureSensorClass
{
public:
    void init(LedController *ledController);
    bool wasPatternEncountered();

private:
    UIController uiController;
    Timer taskTimer;
    void task();
    bool patternEncountered = false;
    bool hasPatternBeenEncountered = false;
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
        DIR_ALL,
        DIR_MAX,
    } Gesture_t;
    String describeGesture(Gesture_t gesture);

    const Gesture_t pattern[6] = {DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_DOWN, DIR_MAX};

    SparkFun_APDS9960 apds;
    void showGestureDirectionForDuration(Gesture_t gesture, uint32_t timeout);
    Timer showGestureDirectionTimer;

    int ledIdForGestureDirection(Gesture_t gesture);
    void startGesturePollingTimer();
    void onPasswordDisplayEnd();
    void startShowingPassword();

    LedController *ledController = nullptr;
};

extern GestureSensorClass GestureSensor;
