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
    // Set drawer open for 5 minutes
    time_t _5_minutes = 5 * 60 * 1000;
    delayDrawerOpenTimer.initializeMs(_5_minutes, &setDrawerClosed).startOnce();
}

void drawerInit()
{
    pinMode(0, OUTPUT);
    digitalWrite(0, OPEN);

    // setDrawer Open for 10 seconds at boot
    delayDrawerOpenTimer.initializeMs(10000, &setDrawerClosed).startOnce();
}
