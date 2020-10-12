#pragma once

#include <SmingCore.h>
#include <Libraries/MCP23017/MCP23017.h>

class Debouncer
{
private:
    int isPushed = 0;

public:
    void init();
    void update(bool val);
    bool getIsPushed();
};

class PasswordValidator
{
    uint8_t password[4] = {3, 8, 7, 3};
    int passwordIdx = 0;
    bool isCorrect = false;

public:
    void setPassword(uint8_t password[4]);
    void pushFigure(int figure);
    void reset();
    bool getIsCorrect();
};

class Key
{
private:
    bool wasPushed = false;
    Debouncer debouncer;
    int figure = -1;
    bool isReleased();
    std::function<void(int)> onReleasedUserCb = nullptr;

public:
    void init();
    void setOnReleasedUserCallback(std::function<void(int)> cb);
    void setFigure(int figure);
    void setPushed(bool isPushed);
};

class KeyPadClass
{
public:
    void init();
    void newUserInput(int figure);
    void reset();

private:
    void task();
    MCP23017 mcp;
    Timer timer;
    Key keys[10];
    PasswordValidator passwordValidator;
};

extern KeyPadClass KeyPad;
