#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"

static Timer heartBeatTimer;

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(HEARTBEAT_TIME_MS, sendHeartBeat).start();
}
