#pragma once

#include "node.h"

class NodeLightRoomClass : public Node
{
private:
    static const int ON;
    static const int OFF;
    int pinStatus = NodeLightRoomClass::OFF;
    int lightGpio_ = 0;

private:
    int toggle();
    int turnOn();
    int turnOff();

public:
    void init();
    void actuate();
};

extern NodeLightRoomClass NodeLightRoom;
