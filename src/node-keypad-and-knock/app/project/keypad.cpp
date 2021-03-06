#include "keypad.h"
#include "tasks.h"
#include "knocker.h"

KeyPadClass KeyPad;

void Debouncer::init()
{
    isPushed = 0;
}

void Debouncer::update(bool val)
{
    if (val)
    {
        isPushed++;
        if (isPushed >= 10)
        {
            isPushed = 10;
        }
    }
    else
    {
        isPushed--;
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

void Key::init()
{
    wasPushed = false;
    figure = -1;
    debouncer.init();
}

bool Key::isReleased()
{
    if (wasPushed && !debouncer.getIsPushed())
    {
        Serial.printf("!! %d pressed !! \n", figure);
        return true;
    }
    return false;

}

void Key::setFigure(int figure)
{
    this->figure = figure;
}

void Key::setOnReleasedUserCallback(std::function<void(int)> cb)
{
    onReleasedUserCb = cb;
}

void Key::setPushed(bool isPushed)
{
    debouncer.update(isPushed);

    if (isReleased())
    {
        if (onReleasedUserCb != nullptr)
        {
            onReleasedUserCb(this->figure);
        }
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
        keys[i].setOnReleasedUserCallback(std::bind(&KeyPadClass::newUserInput, this, std::placeholders::_1));
        mcp.pinMode(i, INPUT);
        mcp.pullUp(i, 1);
    }
    timer.initializeMs(10, std::bind(&KeyPadClass::task, this)).startOnce();
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

void KeyPadClass::newUserInput(int figure)
{
    passwordValidator.pushFigure(figure);
    if (passwordValidator.getIsCorrect())
    {
        nodeKeyPadAndKnock.setSolved(true).sendUpdateNow();
        uint16_t pattern[] = {400, 200, 200, 400, 800, 400};
        patternKnocker.setPattern(pattern, 6);
        patternKnocker.run();
        patternKnocker.setOnDoneUserCallback(std::bind(&KeyPadClass::reset, &KeyPad));
    }
}

void KeyPadClass::reset()
{
    passwordValidator.reset();
}

void PasswordValidator::setPassword(uint8_t password[4])
{
    memcpy(this->password, password, 4);
}

void PasswordValidator::reset()
{
    isCorrect = false;
    passwordIdx = 0;
}

void PasswordValidator::pushFigure(int figure)
{
    if (isCorrect)
    {
        return;
    }

    if (figure == password[passwordIdx])
    {
        passwordIdx++;
    }
    else
    {
        passwordIdx = 0;
    }

    if (passwordIdx >= sizeof(password))
    {
        isCorrect = true;
    }
}

bool PasswordValidator::getIsCorrect()
{
    return isCorrect;
}
