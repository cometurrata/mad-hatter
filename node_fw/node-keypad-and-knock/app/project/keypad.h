#pragma once

#include <SmingCore.h>
#include <Libraries/MCP23017/MCP23017.h>
#include "knocker.h"

class Debouncer
{
private:
    int isPushed = 0;
public:
    void init() {isPushed = 0;}
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

    void init() {wasPushed = false; figure = -1; debouncer.init();}
    void setFigure(int figure);
    void setPushed(bool isPushed);
};

class KeyPadClass
{
public:
    void init();

private:
    void task();
    MCP23017 mcp;
    Timer timer;
    Key keys[10];
};

extern KeyPadClass keyPad;
