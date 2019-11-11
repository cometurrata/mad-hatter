#pragma once

#include <SmingCore.h>
#include <Libraries/MCP23017/MCP23017.h>

class Debouncer
{
private:
    int isPushed = 5;

public:
    void update(bool val);
    bool getIsPushed();
};

class Key
{
private:
    bool wasPushed = false;
    Debouncer debouncer;
    int figure = -1;
    bool isReleased();

public:
    Key(int figure);
    Key();

    void setFigure(int figure);
    void setPushed(bool isPushed);
};

class KeyPadClass
{
public:
    KeyPadClass();

private:
    void task();
    MCP23017 mcp;
    Timer procTimer;
    Key keys[9];
};

extern KeyPadClass keyPad;
