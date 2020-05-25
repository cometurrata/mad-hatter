#include "gesturesensor.h"

GestureSensorClass GestureSensor;

void GestureSensorClass::startPasswordTimer(int timeout)
{
    showPasswordTimer.initializeMs(timeout, std::bind(&GestureSensorClass::showPasswordTask, this)).startOnce();
}

void GestureSensorClass::startShowingPassword()
{
    if (ledController)
        ledController->turnOff();

    showGestureDirectionTimer.stop();

    showPasswordStep = 0;
    startPasswordTimer(10);
}

void GestureSensorClass::showPasswordTask()
{
    if (showPasswordStep >= 4)
    {
        // If done showing password, turnoff leds
        if (ledController)
            ledController->turnOff();

        showPasswordStep = 0;
        patternEncountered = false;
    }
    else
    {
        if (ledController)
            ledController->turnOn(password[showPasswordStep]);

        showPasswordStep++;
        startPasswordTimer(1000);
    }
}

String GestureSensorClass::describeGesture(Gesture_t gesture)
{
    switch (gesture)
    {
    case DIR_NONE:
        return String("DIR_NONE");
        break;
    case DIR_LEFT:
        return String("DIR_LEFT");
        break;
    case DIR_RIGHT:
        return String("DIR_RIGHT");
        break;
    case DIR_UP:
        return String("DIR_UP");
        break;
    case DIR_DOWN:
        return String("DIR_DOWN");
        break;
    case DIR_NEAR:
        return String("DIR_NEAR");
        break;
    case DIR_FAR:
        return String("DIR_FAR");
        break;
    default:
        return String("DIR_ALL");
        break;
    }
}

int GestureSensorClass::ledIdForGestureDirection(Gesture_t gesture)
{
    switch (gesture)
    {
    case DIR_LEFT:
        return LED_LEFT;
    case DIR_RIGHT:
        return LED_RIGHT;
    case DIR_UP:
        return LED_UP;
    case DIR_DOWN:
        return LED_DOWN;
    default:
        return -1;
    }
}

void GestureSensorClass::showGestureDirectionForDuration(Gesture_t gesture, uint32_t timeout)
{
    ledController->turnOff();
    auto cb = std::bind(static_cast<void (LedController::*)(void)>(&LedController::turnOff),
                        ledController);

    showGestureDirectionTimer.initializeMs(timeout, cb).startOnce();
    int ledId = ledIdForGestureDirection(gesture);
    if (ledId >= 0) {
        ledController->turnOn(ledId);
    }
}

void GestureSensorClass::task()
{
    if (patternEncountered)
    {
        return;
    }

    if (!apds.isGestureAvailable())
    {
        Serial.println("Here");
        return;
    }

    Gesture_t gesture = (Gesture_t)apds.readGesture();


    if (gesture >= DIR_NEAR || gesture == DIR_NONE)
    {
        return;
    }

    showGestureDirectionForDuration(gesture, 1000);

    if (pattern[nextGestureIndex] == gesture)
    {
        Serial.println("Good gesture");
        nextGestureIndex++;
    }
    else
    {
        nextGestureIndex = 0;
        Serial.println("Wrong gesture, reseting pattern");
    }

    if (pattern[nextGestureIndex] == DIR_MAX)
    {
        Serial.println("Success");
        patternEncountered = true;
        nextGestureIndex = 0;
        startShowingPassword();
    }
}

void GestureSensorClass::init(LedController *ledController)
{
    this->ledController = ledController;
    Wire.pins(5, 4); // SDA, SCL

    // Initialize APDS-9960 (configure I2C and initial values)
    if (apds.init())
    {
        Serial.println("APDS-9960 initialization complete");
    }
    else
    {
        Serial.println("Something went wrong during APDS-9960 init!");
    }

    // Start running the APDS-9960 gesture sensor engine
    if (apds.enableGestureSensor(true))
    {
        Serial.println("Gesture sensor is now running");
    }
    else
    {
        Serial.println("Something went wrong during gesture sensor init!");
    }

    taskTimer.initializeMs(20, std::bind(&GestureSensorClass::task, this)).start();
}
