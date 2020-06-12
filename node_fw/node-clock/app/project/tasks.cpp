#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "clock.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

void sensorTask()
{
    bool val = clockGetVal();
    #ifdef VERBOSE
    	debugf("resolved : %s", val ? "true" : "false");
	#endif
    if (val)
    {
        sendNodeUpdate("Clock is Well set");
    }
}

void nodeClockInit(void)
{
    clockInit();
    sensorTimer.initializeMs(300, sensorTask).start();
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(HEARTBEAT_TIME_MS, sendHeartBeat).start();
}
