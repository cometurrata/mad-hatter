#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "drawer.h"

static Timer delayDrawerOpenTimer;

void setDrawerClosed()
{
    digitalWrite(0, HIGH);
}

void setDrawerOpen()
{
    digitalWrite(0, LOW);
    delayDrawerOpenTimer.initializeMs(30000, &setDrawerClosed).startOnce();
}

void drawerInit()
{
	pinMode(0, OUTPUT);
    digitalWrite(0, LOW);
    delayDrawerOpenTimer.initializeMs(10000, &setDrawerClosed).startOnce();
}
