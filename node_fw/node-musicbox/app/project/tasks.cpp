#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "musicbox.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

void sensorTask()
{
    if (MusicBox.isMelodyCorrect())
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
