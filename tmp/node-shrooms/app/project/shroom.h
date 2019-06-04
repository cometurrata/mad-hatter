#ifndef SHROOM_H
#define SHROOM_H

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>
// Which pin on the Esp8266 is connected to the NeoPixels?
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;

class Shroom
{
    public:
        Adafruit_NeoPixel *pixels;
        int numpixels;
        int ledPin;
        int touchPin;
        String name;


        bool shroomTouched = false;
        bool shroomTouchedProcessed = false;

        Timer pollTouchTimer;
        Timer lightTaskTimer;

        uint8_t r;
        uint8_t g;
        uint8_t b;

        void init(int touchPin, int ledPin, int numpixels, String name)
        {
            this->ledPin = ledPin;
            this->touchPin = touchPin;
            this->numpixels = numpixels;
            this->name = name;
            this->pixels = new Adafruit_NeoPixel(numpixels, ledPin, NEO_GRB + NEO_KHZ800);
            this->pixels->begin();
            mcp.pinMode(touchPin, INPUT);
            setAllPixelsToColor(Adafruit_NeoPixel::Color(0, 0, 0));
            pollTouchTimer.initializeMs(50, TimerDelegate(&Shroom::pollTouchTask, this)).start();
            lightTaskTimer.initializeMs(105, TimerDelegate(&Shroom::lightTask, this)).start();
        }

        void pollTouchTask()
        {
            if(mcp.digitalRead(touchPin))
            {
                shroomTouched = true;
            }
            else
            {
                shroomTouched = false;
            }
        }

        void lightTask()
        {
            if(shroomTouched)
            {
                r = 50;
                shroomTouched = false;
                debugf("%s Touched", name.c_str() );
            }
            else if (!shroomTouched)
            {
                if (r > 0 )
                {
                    r--;
                }

            }
            setAllPixelsToColor(Adafruit_NeoPixel::Color(r, r, r));
        }

        void setAllPixelsToColor(uint32_t color)
        {
            for (int i = 0 ; i <pixels->numPixels() ; i++)
            {
                pixels->setPixelColor(i, color);
            }
            pixels->show();
        }

};

#endif