#include "keypad.h"
#include "knocker.h"

KeyPadClass keyPad;

void Debouncer::update(bool val)
{
    if (val)
    {
        isPushed++;
        if ( isPushed >= 10)
        {
            isPushed = 10;
        }
    }
    else
    {
        isPushed --;
        if (isPushed <= 0)
        {
            isPushed = 0;
        }
    }

}

bool Debouncer::getIsPushed()
{
    if (isPushed >= 5)
    {
        return true;
    }
    return false;
}

bool Key::isReleased()
{
    return (wasPushed && !debouncer.getIsPushed());
}

void Key::setFigure(int figure)
{
    this->figure = figure;
}

void Key::setPushed(bool isPushed)
{
    debouncer.update(isPushed);

    if (isReleased())
    {
        knocker.start(this->figure);
    }

    wasPushed = debouncer.getIsPushed();
}

void KeyPadClass::init()
{
    debugf("Init MCP");
    Wire.pins(4, 5); // SDA, SCL
    mcp.begin();
    for (uint8_t i = 0; i < 10; i++)
    {
        keys[i].init();
        keys[i].setFigure((uint8_t)i);
        mcp.pinMode(i, INPUT);
        mcp.pullUp(i, 1);
    }
    knocker.init();
    timer.initializeMs(20, std::bind(&KeyPadClass::task, this)).startOnce();
}

void KeyPadClass::task()
{
    for (uint8_t i = 0; i < 10; i++)
    {
        uint8_t pinV = !mcp.digitalRead(i);
        keys[i].setPushed(pinV);
    }
    timer.startOnce();
}
