#include "shroom.h"

void Shroom::init(Adafruit_MCP23017 &mcp, int touchPin, int ledPin, int numpixels, String name, std::function<void(String)> callback)
{
    this->ledPin = ledPin;
    this->touchPin = touchPin;
    this->numpixels = numpixels;
    this->name = name;
    this->onTouchCallback = callback;
    this->pixels = new Adafruit_NeoPixel(numpixels, ledPin, NEO_GRB + NEO_KHZ800);
    this->pixels->begin();
    this->mcp = mcp;
    this->mcp.pinMode(touchPin, INPUT);
    setAllPixelsToColor(Adafruit_NeoPixel::Color(0, 0, 0));
    pollTouchTimer.initializeMs(50, TimerDelegate(&Shroom::pollTouchTask, this)).start();
    lightTaskTimer.initializeMs(105, TimerDelegate(&Shroom::lightTask, this)).start();
}

void Shroom::pollTouchTask()
{

    if (mcp.digitalRead(touchPin))
    {
    	if (!shroomTouched)
    	{
    		onTouchCallback(name);
    	}
        shroomTouched = true;
    }
    else
    {
        shroomTouched = false;
    }
    //debugf("id: %s shroomTouched: %s\n", name.c_str(), shroomTouched ? "true" : "false");
}

void Shroom::lightTask()
{
    if (shroomTouched)
    {
        r = 50;
    }
    else
    {
        if (r > 0)
        {
            r--;
        }
    }
    setAllPixelsToColor(Adafruit_NeoPixel::Color(r, r, r));
}

void Shroom::setAllPixelsToColor(uint32_t color)
{
    for (int i = 0; i < pixels->numPixels(); i++)
    {
        pixels->setPixelColor(i, color);
    }
    pixels->show();
}