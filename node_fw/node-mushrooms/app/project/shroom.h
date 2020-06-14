#ifndef SHROOM_H
#define SHROOM_H

#include <SmingCore.h>

#include <Libraries/Adafruit_NeoPixel/Adafruit_NeoPixel.h>
#include "Adafruit_MCP23017.h"

class Shroom
{
public:
    Adafruit_MCP23017 mcp;
    Adafruit_NeoPixel *pixels;
    int numpixels;
    int ledPin;
    int touchPin;
    String name;

    std::function<void(String)> onTouchCallback;

    bool shroomTouched = false;
    bool shroomTouchedProcessed = false;

    Timer pollTouchTimer;
    Timer lightTaskTimer;

    uint8_t r;
    uint8_t g;
    uint8_t b;

    void init(Adafruit_MCP23017 &mcp,int touchPin, int ledPin, int numpixels, String name, std::function<void(String)> callback);

    void pollTouchTask();

    void lightTask();

    void setAllPixelsToColor(uint32_t color);
};

#endif