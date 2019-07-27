#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "http_client.h"
#include "drawer.h"

static Timer heartBeatTimer;
static Timer delayDrawerOpenTimer;

void setDrawerClosed()
{
    digitalWrite(0, HIGH);
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(15 * 1000, &sendHeartBeat).start();
    pinMode(0, OUTPUT);
    digitalWrite(0, LOW);
    delayDrawerOpenTimer.initializeMs(10000, &setDrawerClosed).startOnce();
}
