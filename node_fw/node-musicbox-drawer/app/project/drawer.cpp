#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "drawer.h"

#define OPEN LOW
#define CLOSED HIGH

static Timer delayDrawerOpenTimer;

void setDrawerClosed()
{
    digitalWrite(0, CLOSED);
}

void setDrawerOpen()
{
    digitalWrite(0, OPEN);
    delayDrawerOpenTimer.initializeMs(30000, &setDrawerClosed).startOnce();
}

void drawerInit()
{
    pinMode(0, OUTPUT);
    digitalWrite(0, OPEN);

    // setDrawer Open for 10 seconds at boot
    delayDrawerOpenTimer.initializeMs(10000, &setDrawerClosed).startOnce();
}
