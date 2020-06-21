#include <stdint.h>
#include <SmingCore.h>

#include "tasks.h"
#include "http_client.h"
#include "clock.h"

Node NodeClock;
static Timer sensorTimer;

void sensorTask()
{
    bool val = clockGetVal();
    #ifdef VERBOSE
    	debugf("resolved : %s", val ? "true" : "false");
	#endif
    if(val)
    {
        NodeClock.setSolved(true).sendUpdateNow();
    }
}

void nodeClockInit(void)
{
    clockInit();
    sensorTimer.initializeMs(300, sensorTask).start();
}

