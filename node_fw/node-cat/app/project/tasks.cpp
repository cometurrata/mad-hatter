#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"

static Timer heartBeatTimer;

uint32_t threshold = 15000;

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(HEARTBEAT_TIME_MS, sendHeartBeat).start();
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
}
