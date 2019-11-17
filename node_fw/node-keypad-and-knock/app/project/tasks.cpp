#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "keypad.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

void nodeClockInit(void)
{
    keyPad.init();
}

void nodeHeartBeatInit()
{
    // heartBeatTimer.initializeMs(30 * 1000, sendHeartBeat);
}
