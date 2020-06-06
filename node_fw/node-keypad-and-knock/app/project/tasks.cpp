#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "keypad.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

void nodeKeypadInit(void)
{
    keyPad.init();
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(HEARTBEAT_TIME_MS, sendHeartBeat);
}
