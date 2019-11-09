#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "clock.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

int32_t threshold = -32500;

void sensorTask()
{
    int32_t val = clockGetVal();
    debugf("val:%lu", val);
    if (val < threshold)
    {
        sendNodeUpdate("Clock is Well set");
    }
}

void nodeClockInit(void)
{
    clockInit();
    sensorTimer.initializeMs(3 * 1000, sensorTask).start();
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, sendHeartBeat);
}
