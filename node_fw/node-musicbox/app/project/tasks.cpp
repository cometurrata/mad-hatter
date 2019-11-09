#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
// #include "clock.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

uint32_t threshold = 15000;

void sensorTask()
{
    uint32_t val = clockGetVal();

    if (val > threshold)
    {
        sendNodeUpdate("Clock is Well set");
    }
}

void nodeClockInit(void)
{
    sensorTimer.initializeMs(3 * 1000, sensorTask).start();
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, sendHeartBeat);
}
