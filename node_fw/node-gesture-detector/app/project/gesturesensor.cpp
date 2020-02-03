#include "gesturesensor.h"

GestureSensorClass GestureSensor;

SparkFun_APDS9960 apds = SparkFun_APDS9960();

void GestureSensorClass::startShowingPassword()
{
    if (charlieplexing)
        charlieplexing->turnOff();

    showPasswordStep = 0;
    showPasswordTimer.initializeMs(10, std::bind(&GestureSensorClass::showPasswordTask, this)).startOnce();
}

void GestureSensorClass::showPasswordTask()
{
    if (showPasswordStep >= 4)
    {
        // If done showing password, turnoff leds
        if (charlieplexing)
            charlieplexing->turnOff();

        showPasswordStep = 0;
        patternEncountered = false;
    }
    else
    {
        if (charlieplexing)
            charlieplexing->setLed(password[showPasswordStep]);

        showPasswordStep++;
        showPasswordTimer.initializeMs(1000, std::bind(&GestureSensorClass::showPasswordTask, this)).startOnce();
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

    Serial.printf("Gesture: %d\n", gesture);

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

    if (nextGestureIndex >= sizeof(pattern))
    {
        Serial.println("Success");
        patternEncountered = true;
        nextGestureIndex = 0;
        startShowingPassword();
    }
}

void GestureSensorClass::init(CharliePlexing *charlieplexing)
{
    this->charlieplexing = charlieplexing;

    if (charlieplexing)
        charlieplexing->init();

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
