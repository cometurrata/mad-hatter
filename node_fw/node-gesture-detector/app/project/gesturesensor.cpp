#include "gesturesensor.h"

GestureSensorClass GestureSensor;

uint8_t LEDS[4] = {1, 2, 3, 6};

SparkFun_APDS9960 apds = SparkFun_APDS9960();

void GestureSensorClass::turnOnLed(int pin)
{
    debugf("Turning pin %d ON", pin);
    return;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

void GestureSensorClass::turnOffLed(int pin)
{
    debugf("Turning pin %d Off", pin);
    return;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void GestureSensorClass::startShowingPassword()
{
    for (int i = 0; i < 4; i++)
    {
        turnOffLed(LEDS[i]);
    }
    showPasswordStep = 0;
    showPasswordTimer.initializeMs(10, std::bind(&GestureSensorClass::showPasswordTask, this)).startOnce();
}

void GestureSensorClass::showPasswordTask()
{
    for (int i = 0; i < 4; i++)
    {
        if (i == showPasswordStep)
        {
            turnOnLed(LEDS[i]);
        }
        else
        {
            turnOffLed(LEDS[i]);
        }
    }

    showPasswordStep++;

    if (showPasswordStep >= 4) // Run one more time to be sure every led is turned off
    {
        nextGestureIndex = 0;
        showPasswordStep = 0;
        patternEncountered = false;
    }
    else
    {
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
        startShowingPassword();
    }
}

void GestureSensorClass::init()
{
    Serial.println();
    Serial.println("--------------------------------------");
    Serial.println("-- SparkFun APDS-9960 - GestureTest --");
    Serial.println("--------------------------------------");

    for (int i = 0; i < 4; i++)
    {
        turnOffLed(LEDS[i]);
    }

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
