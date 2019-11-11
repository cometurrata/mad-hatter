#include "keypad.h"
#include "knocker.h"

KeyPadClass keyPad = KeyPadClass();

void Debouncer::update(bool val)
{
    if (val)
    {
        if (++isPushed >= 10)
        {
            isPushed = 10;
        }
    }
    else
    {
        if (--isPushed <= 0)
        {
            isPushed = 0;
        }
    }
}

bool Debouncer::getIsPushed()
{
    return isPushed >= 5;
}

bool Key::isReleased()
{
    return (wasPushed && !debouncer.getIsPushed());
}

Key::Key(int figure)
{
    this->figure = figure;
}

Key::Key() {}

void Key::setFigure(int figure)
{
    this->figure = figure;
}

void Key::setPushed(bool isPushed)
{
    debouncer.update(isPushed);

    if (isReleased())
    {
        knocker.start(figure);
    }

    wasPushed = debouncer.getIsPushed();
}

KeyPadClass::KeyPadClass()
{
    for (int i = 0; i < 10; i++)
    {
        keys[i].setFigure(i);
    }

    Wire.pins(4, 5); // SDA, SCL
    mcp.begin();
    procTimer.initializeMs(20, std::bind(&KeyPadClass::task, this)).start();
}

void IRAM_ATTR KeyPadClass::task()
{
    for (int figure = 0; figure <= 9; figure++)
    {
        keys[figure].setPushed(mcp.digitalRead(figure));
    }
}
