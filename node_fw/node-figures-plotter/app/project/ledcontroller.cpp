#include "ledcontroller.h"

static LedController ledControllerinstance;

LedController *LedController::instance()
{
    return &ledControllerinstance;
}

void LedController::init()
{
    Wire.pins(4, 5); // SDA, SCL
    mcp.begin();
    for (int i = 0; i < 16; i++)
    {
        mcp.pinMode(i, OUTPUT);
        // mcp.pullUp(i, HIGH);
        mcp.digitalWrite(i, LOW);
    }
}

void LedController::setLightUpDuration(int duration)
{
    lightUpDuration = duration;
}

void LedController::lightUpFigure(int figure)
{
    timer.stop();
    turnOffLed(lastLightenUpLed);
    mcp.digitalWrite(pins[figure], HIGH);
    Serial.printf("lighting up %d\n", pins[figure]);
    lastLightenUpLed = pins[figure];
    // timer.initializeMs(lightUpDuration, std::bind(&LedController::turnOffLastLightenLed, this)).startOnce();
}

void LedController::turnOffLastLightenLed()
{
    turnOffLed(lastLightenUpLed);
}

void LedController::turnOffLed(int pin)
{
    mcp.digitalWrite(pin, LOW);
}