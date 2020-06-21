#pragma once

#include <SmingCore.h>
#include "Adafruit_MCP23017.h"
#include "http_client.h"

class LedController
{
    Adafruit_MCP23017 mcp;
    Timer timer;
    int lastLightenUpLed;
    uint32_t lightUpDuration = 2000;
    const int pins[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

public:
    static LedController *instance();
    void init();
    void setLightUpDuration(int duration = 2000);
    void lightUpFigure(int figure);
    void turnOffLastLightenLed();
    void turnOffLed(int pin);
};
