#include "gesturesensor.h"
#include "tasks.h"

GestureSensorClass GestureSensor;

bool GestureSensorClass::wasPatternEncountered()
{
    return hasPatternBeenEncountered;
}

void GestureSensorClass::startGesturePollingTimer(void)
{
    taskTimer.initializeMs(20, std::bind(&GestureSensorClass::task, this)).startOnce();
}

void GestureSensorClass::onPasswordDisplayEnd()
{
    patternEncountered = false;
    startGesturePollingTimer();
}

void GestureSensorClass::startShowingPassword()
{
    uiController.startShowingPassword(std::bind(&GestureSensorClass::onPasswordDisplayEnd, this));
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
    if (ledId >= 0)
    {
        ledController->turnOn(ledId);
    }
}

void GestureSensorClass::task()
{
    Gesture_t gesture;

    if (patternEncountered)
    {
        Serial.println("[GESTURE_SENSOR] Pattern encountered !");
        return;
    }

    if (!apds.isGestureAvailable())
    {
        goto exit;
    }

    gesture = (Gesture_t)apds.readGesture();
    Serial.print("[GESTURE_SENSOR] gesture read = ");
    Serial.print(describeGesture(gesture));
    Serial.printf(" (%d)", gesture);
    Serial.println("");

    if (gesture >= DIR_NEAR || gesture == DIR_NONE)
    {
        Serial.println("[GESTURE_SENSOR] Invalid gesture");
        goto exit;
    }

    Serial.println("[GESTURE_SENSOR] Show gesture");
    showGestureDirectionForDuration(gesture, 1000);

    if (pattern[nextGestureIndex] == gesture)
    {
        Serial.println("[GESTURE_SENSOR] Good gesture");
        nextGestureIndex++;
    }
    else
    {
        nextGestureIndex = 0;
        Serial.println("[GESTURE_SENSOR] Wrong gesture, reseting pattern");
    }

    if (pattern[nextGestureIndex] == DIR_MAX)
    {
        // TODO: Notify Server that gesture was properly detected
        Serial.println("[GESTURE_SENSOR] Success !!");
        patternEncountered = true;
        hasPatternBeenEncountered = true;
        nextGestureIndex = 0;
        startShowingPassword();
        nodeGestureDetector.setSolved(true).sendUpdateNow();
    }

exit:
    startGesturePollingTimer();
}

void GestureSensorClass::init(LedController *ledController)
{
    this->ledController = ledController;
    Wire.pins(5, 4); // SDA, SCL
    uiController.init(ledController);

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
    if (apds.enableGestureSensor(false))
    {
        Serial.println("Gesture sensor is now running");
    }
    else
    {
        Serial.println("Something went wrong during gesture sensor init!");
    }

    startGesturePollingTimer();
}
