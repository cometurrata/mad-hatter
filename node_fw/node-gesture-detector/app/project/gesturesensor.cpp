#include "gesturesensor.h"

GestureSensorClass GestureSensor;

void GestureSensorClass::startShowingPassword()
{
    if (ledController)
        ledController->turnOff();

    showGestureDirectionTimer.stop();

    showPasswordStep = 0;
    showPasswordTimer.initializeMs(10, std::bind(&GestureSensorClass::showPasswordTask, this)).startOnce();
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
        showPasswordTimer.initializeMs(1000, std::bind(&GestureSensorClass::showPasswordTask, this)).startOnce();
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

void GestureSensorClass::showGestureDirectionForDuration(Gesture_t gesture, uint32_t timeout)
{
    ledController->turnOff();
    auto cb = std::bind(static_cast<void (LedController::*)(void)>(&LedController::turnOff),
                        ledController);

    showGestureDirectionTimer.initializeMs(timeout, cb).startOnce();
    switch (gesture)
    {
    case DIR_LEFT:
        ledController->turnOn(10);
        break;
    case DIR_RIGHT:
        ledController->turnOn(11);
        break;
    case DIR_UP:
        ledController->turnOn(12);
        break;
    case DIR_DOWN:
        ledController->turnOn(13);
        break;
    default:
        break;
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
        return;
    }

    Gesture_t gesture = (Gesture_t)apds.readGesture();

    Serial.printf("Gesture: %s\n", describeGesture(gesture).c_str());
    showGestureDirectionForDuration(gesture, 1000);

    if (gesture >= DIR_NEAR || gesture == DIR_NONE)
    {
        Serial.println("Ignore gesture");
    }
    else if (pattern[nextGestureIndex] == gesture)
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
