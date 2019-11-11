#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "keypad.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

void sensorTask()
{
}

void nodeClockInit(void)
{
    sensorTimer.initializeMs(3 * 1000, sensorTask).start();
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, sendHeartBeat);
}
