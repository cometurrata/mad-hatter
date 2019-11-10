#include "gesturesensor.h"

GestureSensorClass GestureSensor;

SparkFun_APDS9960 apds = SparkFun_APDS9960();

void GestureSensorClass::task()
{
    if (!apds.isGestureAvailable())
    {
        return;
    }

    Gesture_t gesture = (Gesture_t)apds.readGesture();

    if (gesture >= DIR_NEAR || gesture == DIR_NONE)
    {
        Serial.println("Ignore gesture");
    }
    else if (pattern[nextGestureIndex] == gesture)
    {
        nextGestureIndex++;
    }
    else
    {
        nextGestureIndex = 0;
        Serial.println("Wrong gesture, reseting pattern");
    }

    if (nextGestureIndex >= sizeof(pattern))
    {
        patternEncountered = true;
    }
}

void GestureSensorClass::init()
{
    Serial.println();
    Serial.println("--------------------------------------");
    Serial.println("-- SparkFun APDS-9960 - GestureTest --");
    Serial.println("--------------------------------------");

    Wire.pins(4, 5); // SDA, SCL

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
